//
//  cloud.c
//  obel
//
//  Created by Jack Mousseau on 9/23/21.
//

#include <obel/cloud.h>
#include <obel/coord.h>

// MARK: - Allocation

OBEL_CLOUD_FUNC int32_t obel_cloud_map(obel_cloud_t *cloud) {
    if (cloud->capacity == 0) {
        return -1;
    }

    size_t length = cloud->capacity * sizeof(obel_vector3_t);
    obel_vector3_t *points = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (points == MAP_FAILED) {
        return -1;
    }

    cloud->count = 0;
    cloud->points = points;
    return 0;
}

OBEL_CLOUD_FUNC int32_t obel_cloud_unmap(obel_cloud_t *cloud) {
    if (munmap(cloud->points, cloud->capacity * sizeof(obel_vector3_t)) < 0) {
        return -1;
    }

    cloud->count = 0;
    cloud->capacity = 0;
    return 0;
}

// MARK: - File I/O

OBEL_CLOUD_FUNC int32_t obel_cloud_read_bin(int fd, obel_cloud_t *cloud) {
    stat_t stat;
    if (fstat(fd, &stat) < 0) {
        return -1;
    }

    size_t count = stat.st_size / sizeof(obel_vector3_t);
    if (count > UINT32_MAX) {
        return -1;
    }

    cloud->capacity = (uint32_t)count;

    size_t length = cloud->capacity * sizeof(obel_vector3_t);
    obel_vector3_t *points = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (points == MAP_FAILED) {
        return -1;
    }

    cloud->count = cloud->capacity;
    cloud->points = points;
    return 0;
}

OBEL_CLOUD_FUNC int32_t obel_cloud_read_xyz(int fd, obel_cloud_t *cloud) {
    size_t size = 4096;
    size_t count = 0;
    char buffer[size];

    uint32_t spaces = 0;
    while ((count = read(fd, &buffer, size)) > 0) {
        for (uint32_t i = 0; i < count; i++) {
            switch (buffer[i]) {
                case ' ':
                    spaces++;
                    break;
                case '\n':
                    cloud->capacity++;
                    spaces = 0;
                    break;
                default:
                    break;
            }
        }
    }

    if (spaces == 2) {
        cloud->capacity++;
    }

    if (count < 0) { return -1; };
    if (lseek(fd, 0, SEEK_SET) < 0) { return -1; };
    if (obel_cloud_map(cloud) < 0) {  return -1; };

    float *points = (float *)cloud->points;
    while ((count = read(fd, &buffer, size)) > 0) {
        size_t complete = count;

        if (count < size) {
            buffer[complete] = ' ';
        } else {
            while (buffer[--complete] != ' ') {}
        }

        char *head = &buffer[0];
        char *tail = &buffer[complete];
        while (head != tail) {
            *(points++) = strtod(head, &head);
        }

        lseek(fd, complete - count, SEEK_CUR);
    }

    cloud->count = cloud->capacity;
    return 0;
}

OBEL_CLOUD_FUNC int32_t obel_cloud_write_bin(int fd, obel_cloud_t *cloud) {
    size_t length = cloud->count * sizeof(obel_vector3_t);
    if (write(fd, cloud->points, length) != length) { return -1; }
    return 0;
}

OBEL_CLOUD_FUNC int32_t obel_cloud_write_ply(int fd, obel_cloud_t *cloud) {
    if (dprintf(fd, "ply\n") < 0) { return -1; };
    if (dprintf(fd, "format ascii 1.0\n") < 0) { return -1; };
    if (dprintf(fd, "element vertex %u\n", cloud->count) < 0) { return -1; };
    if (dprintf(fd, "property float x\n") < 0) { return -1; };
    if (dprintf(fd, "property float y\n") < 0) { return -1; };
    if (dprintf(fd, "property float z\n") < 0) { return -1; };
    if (dprintf(fd, "end_header\n") < 0) { return -1; };
    if (obel_cloud_write_xyz(fd, cloud) < 0) { return -1; };
    return 0;
}

OBEL_CLOUD_FUNC int32_t obel_cloud_write_xyz(int fd, obel_cloud_t *cloud) {
    for (uint32_t i = 0; i < cloud->count; i++) {
        obel_vector3_t point = cloud->points[i];
        if (dprintf(fd, "%.6f %.6f %.6f\n", point.x, point.y, point.z) < 0) { return -1; };
    }

    return 0;
}

// MARK: - Simplification

OBEL_CLOUD_FUNC void obel_cloud_simplify(obel_arena_t *arena, obel_cloud_t *cloud, obel_cloud_t *simplified) {
    assert(simplified->capacity < cloud->capacity);

    obel_byte_t *bytes = arena->bytes + arena->count;

    // Stuff to consider/fix...
    //
    //  - Better offset band radius heuristic. Maybe we could use a d2 histogram?
    //  - The size of the grid may need to be enlarged along each axis in order
    //    to fit the offset band.
    //  - Does the grid's stride make sense?
    //  - Determine a more accurate coordinate table upper bound capacity (remember
    //    we want around 65% occupancy max). The current idea is to use the cloud's
    //    count as well as the band's stride and radius.
    //  - Determine tighter capacities for the heaps. For the min heap, consider
    //    how 4/3 * pi * r^2 might be used. For the max heap, consider
    //    band.table.capacity / 3.
    //
    float radius = 0.001f;
    uint32_t table_capacity = 10 * cloud->count;

    obel_aabb_t cloud_aabb = obel_cloud_aabb(cloud);

    obel_coord_offset_band_t band = {
        .radius = radius,
        .grid = obel_coord_grid_make(2.2 * radius, cloud_aabb),
        .table = {
            .capacity = table_capacity,
            .count = 0,
            .coords = obel_arena_push_coord(arena, table_capacity)
        }
    };

    uint32_t *projections = (uint32_t *)obel_arena_push(arena, table_capacity, sizeof(uint32_t));
    uint32_t found_i, found_j = 0;

    obel_coord_neighborhood_t neighborhood = obel_coord_neighborhood_push(arena, band.radius, band.grid.stride);

    uint32_t shared_count = 0;

    for (uint32_t i = 0; i < cloud->count; i++) {
        obel_coord_t coord = obel_coord_grid_bin(&band.grid, cloud->points[i]);
        if (!obel_coord_table_find(&band.table, coord, &found_i)) {
            band.table.coords[found_i] = coord;
            band.table.coords[found_i].bits |= OBEL_COORD_BITS_OCCUPIED;
            band.table.count++;
            projections[found_i] = i;

            for (uint32_t j = 0; j < neighborhood.count; j++) {
                obel_coord_offset_t offset = neighborhood.offsets[j];
                obel_coord_t neighbor = obel_coord_make(coord.x + offset.x, coord.y + offset.y, coord.z + offset.z);
                if (!obel_coord_table_find(&band.table, neighbor, &found_j)) {
                    band.table.coords[found_j] = neighbor;
                    band.table.coords[found_j].bits |= OBEL_COORD_BITS_OCCUPIED;
                    band.table.count++;
                    projections[found_j] = i;
                } else {
                    shared_count += 1;
                }
            }
        } else {
            shared_count += 1;
        }
    }

    obel_coord_marcher_t marcher = {
        .band = band,
        .propagation = {
            .order = obel_heap_order_min,
            .mask = OBEL_COORD_BITS_ARR_TIME,
            .capacity = table_capacity,
            .coords = obel_arena_push_coord(arena, table_capacity)
        },
        .vertices = {
            .order = obel_heap_order_max,
            .mask = OBEL_COORD_BITS_ARR_TIME,
            .capacity = table_capacity,
            .coords = obel_arena_push_coord(arena, table_capacity)
        }
    };

    const uint32_t max_initial_count = 4;
    uint32_t initial_count = 0;
    uint32_t initial_coords[max_initial_count];

    for (uint32_t i = 0; i < max_initial_count; i++) {
        uint32_t rand_i = arc4random_uniform(cloud->count);
        obel_coord_t coord = obel_coord_grid_bin(&band.grid, cloud->points[rand_i]);
        if (obel_coord_table_find(&band.table, coord, &found_i)) {
            // Mark an initial set of points in the offset band as ALIVE.
            band.table.coords[found_i].bits |= OBEL_COORD_BITS_ALIVE;
            initial_coords[initial_count++] = found_i;
            obel_coord_heap_insert(&marcher.propagation, band.table.coords[found_i]);
        } else {
            assert(false);
        }
    }

    obel_coord_propagate(&marcher);

    while (simplified->count < simplified->capacity) {
        uint32_t projection = projections[obel_coord_march(&marcher)];
        simplified->points[simplified->count++] = cloud->points[projection];
    }

    obel_arena_pop(arena, bytes);
}

// MARK: - Statistics

OBEL_CLOUD_FUNC obel_aabb_t obel_cloud_aabb(obel_cloud_t *cloud) {
    obel_aabb_t aabb = OBEL_AABB_EMPTY;
    for (uint32_t i = 0; i < cloud->count; i++) {
        aabb = obel_aabb_union(aabb, cloud->points[i]);
    }

    return aabb;
}

OBEL_CLOUD_FUNC obel_cloud_stat_t obel_cloud_stat(obel_cloud_t *cloud) {
    obel_cloud_stat_t stat = { .center_of_mass = OBEL_VECTOR3_ZERO, .aabb = OBEL_AABB_EMPTY };
    for (uint32_t i = 0; i < cloud->count; i++) {
        obel_vector3_t point = cloud->points[i];
        stat.center_of_mass = obel_vector_add(stat.center_of_mass, point);
        stat.aabb = obel_aabb_union(stat.aabb, point);
    }

    stat.center_of_mass = obel_vector_div(stat.center_of_mass, (float)cloud->count);
    return stat;
}

// MARK: - Shape Distribution

OBEL_CLOUD_FUNC void obel_cloud_l2_histogram_fit(obel_aabb_t aabb, obel_histogram_t *histogram) {
    OBEL_ASSERT(histogram->count > 0);
    histogram->offset = 0.f;
    histogram->stride = obel_vector_distance(aabb.min, aabb.max) / (float)histogram->count;
}

OBEL_CLOUD_FUNC void obel_cloud_l2_histogram_bin(obel_cloud_t *cloud, uint32_t samples, obel_histogram_t *histogram) {
    uint32_t max_samples = OBEL_MIN(samples, cloud->count * (cloud->count - 1) / 2);
    for (uint32_t i = 0; i < max_samples; i++) {
        obel_vector3_t point_a = cloud->points[arc4random_uniform(cloud->count)];
        obel_vector3_t point_b = cloud->points[arc4random_uniform(cloud->count)];
        obel_histogram_bin(histogram, obel_vector_distance(point_a, point_b));
    }
}

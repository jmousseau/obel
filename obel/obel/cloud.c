//
//  cloud.c
//  obel
//
//  Created by Jack Mousseau on 9/23/21.
//

#include <obel/cloud.h>

// MARK: - Allocation

OBEL_CLOUD_FUNC int32_t obel_cloud_map(int fd, obel_cloud_t *cloud) {
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

OBEL_CLOUD_FUNC int32_t obel_cloud_write_xyz(int fd, obel_cloud_t *cloud) {
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

    for (uint32_t i = 0; i < cloud->count; i++) {
        obel_vector3_t point = cloud->points[i];
        if (dprintf(fd, "%.6f %.6f %.6f\n", point.x, point.y, point.z) < 0) { return -1; };
    }

    return 0;
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

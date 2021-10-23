//
//  coord.c
//  obel
//
//  Created by Jack Mousseau on 10/2/21.
//

#include <obel/coord.h>

// MARK: - Sparse Coordinate Table

OBEL_COORD_FUNC uint64_t obel_coord_xyz_hash(obel_coord_t coord) {
    uint64_t hash = ((int64_t)coord.x << 48);
    hash |= ((uint64_t)coord.y << 32);
    hash |= ((uint64_t)coord.z << 16);
    return (OBEL_COORD_BITS_OCCUPIED | (hash & 0x0000ffffffffffff));
}

OBEL_COORD_FUNC bool obel_coord_table_insert(obel_coord_table_t *table, obel_coord_t coord) {
    return obel_coord_table_insert(table, coord, NULL);
}

OBEL_COORD_FUNC bool obel_coord_table_insert(obel_coord_table_t *table, obel_coord_t coord, uint32_t *i) {
    if (table->count == table->capacity) {
        return false;
    }

    uint32_t found_i;
    obel_coord_table_find(table, coord, &found_i);
    table->coords[found_i] = coord;
    table->coords[found_i].bits |= OBEL_COORD_BITS_OCCUPIED;
    table->count++;
    OBEL_SAFE_DEREF_ASSIGN(i, found_i);
    return true;
}

OBEL_COORD_FUNC bool obel_coord_table_remove(obel_coord_table_t *table, obel_coord_t coord) {
    return obel_coord_table_remove(table, coord, NULL);
}

OBEL_COORD_FUNC bool obel_coord_table_remove(obel_coord_table_t *table, obel_coord_t coord, uint32_t *i) {
    uint32_t found_i;
    if (obel_coord_table_find(table, coord, &found_i)) {
        table->coords[found_i].bits &= ~OBEL_COORD_BITS_OCCUPIED;
        table->count--;
        OBEL_SAFE_DEREF_ASSIGN(i, found_i);
        return true;
    } else {
        return false;
    }
}

OBEL_COORD_FUNC bool obel_coord_table_find(obel_coord_table_t *table, obel_coord_t coord) {
    return obel_coord_table_find(table, coord, NULL);
}

OBEL_COORD_FUNC bool obel_coord_table_find(obel_coord_table_t *table, obel_coord_t coord, uint32_t *i) {
    uint32_t current = (uint32_t)(obel_coord_xyz_hash(coord) % (uint64_t)table->capacity);
    uint32_t end = current == 0 ? table->capacity - 1 : current - 1;

    obel_coord_t existing = { 0 };
    while (current != end && (existing = table->coords[current]).bits & OBEL_COORD_BITS_OCCUPIED) {
        if (existing.x == coord.x && existing.y == coord.y && existing.z == coord.z) {
            OBEL_SAFE_DEREF_ASSIGN(i, current);
            return true;
        } else {
            current = (current + 1) < table->capacity ? (current + 1) : 0;
        }
    }

    if (current != end) {
        OBEL_SAFE_DEREF_ASSIGN(i, current);
    }

    return false;
}

// MARK: - Sparse Coordinate Grid

OBEL_COORD_FUNC obel_coord_grid_t obel_coord_grid_make(float stride, obel_aabb_t aabb) {
    obel_coord_t max_coord = obel_coord_grid_bin(stride, aabb.min, aabb.max);
    return (obel_coord_grid_t){
        .origin = aabb.min,
        .stride = stride,
        .width = max_coord.x + 1,
        .height = max_coord.y + 1,
        .depth = max_coord.z + 1
    };
}

OBEL_COORD_FUNC obel_coord_t obel_coord_grid_bin(float stride, obel_vector3_t origin, obel_vector3_t point) {
    obel_vector3_t coord = obel_vector_div(obel_vector_sub(point, origin), stride);
    return obel_coord_make(floorf(coord.x), floorf(coord.y), floorf(coord.z));
}

OBEL_COORD_FUNC obel_coord_t obel_coord_grid_bin(obel_coord_grid_t *grid, obel_vector3_t point) {
    return obel_coord_grid_bin(grid->stride, grid->origin, point);
}

// MARK: - Coordinate Heap

OBEL_COORD_FUNC uint32_t obel_coord_heap_insert(obel_coord_heap_t *heap, obel_coord_t coord) {
    if (heap->count == heap->capacity) {
        return 0;
    }

    uint32_t current = ++heap->count;
    heap->coords[current] = coord;

    if (current == OBEL_HEAP_ROOT) {
        return current;
    }

    uint32_t parent = current;
    switch (heap->order) {
        case obel_heap_order_min: {
            while ((heap->coords[current].bits & heap->mask) <
                   (heap->coords[parent = OBEL_HEAP_PARENT(current)].bits & heap->mask)) {
                obel_swap(&heap->coords[current].bits, &heap->coords[parent].bits);
                current = parent;
            }
            break;
        }

        case obel_heap_order_max: {
            while ((heap->coords[current].bits & heap->mask) >
                   (heap->coords[parent = OBEL_HEAP_PARENT(current)].bits & heap->mask)) {
                obel_swap(&heap->coords[current].bits, &heap->coords[parent].bits);
                current = parent;
            }
            break;
        }
    }

    return current;
}

OBEL_COORD_FUNC obel_coord_t obel_coord_heap_remove(obel_coord_heap_t *heap, uint32_t i) {
    obel_coord_t coord = heap->coords[i];
    heap->coords[i] = heap->coords[heap->count];
    heap->coords[heap->count--] = (obel_coord_t){ 0 };

    uint32_t left, right = 0;
    uint32_t current = i;

    switch (heap->order) {
        case obel_heap_order_min: {
            uint32_t smallest = 0;
            while ((smallest = current) < heap->count) {
                if ((heap->coords[left = OBEL_HEAP_LEFT(current)].bits & heap->mask) <
                    (heap->coords[smallest].bits & heap->mask)) {
                    if (left <= heap->count) {
                        smallest = left;
                    }
                }

                if ((heap->coords[right = OBEL_HEAP_RIGHT(current)].bits & heap->mask) <
                    (heap->coords[smallest].bits & heap->mask)) {
                    if (right <= heap->count) {
                        smallest = right;
                    }
                }

                if (current == smallest) {
                    break;
                }

                obel_swap(&heap->coords[current].bits, &heap->coords[smallest].bits);
                current = smallest;
            }
            break;
        }

        case obel_heap_order_max: {
            uint32_t largest = 0;
            while ((largest = current) < heap->count) {
                if ((heap->coords[left = OBEL_HEAP_LEFT(current)].bits & heap->mask) >
                    (heap->coords[largest].bits & heap->mask)) {
                    if (left <= heap->count) {
                        largest = left;
                    }
                }

                if ((heap->coords[right = OBEL_HEAP_RIGHT(current)].bits & heap->mask) >
                    (heap->coords[largest].bits & heap->mask)) {
                    if (right <= heap->count) {
                        largest = right;
                    }
                }

                if (current == largest) {
                    break;
                }

                obel_swap(&heap->coords[current].bits, &heap->coords[largest].bits);
                current = largest;
            }
            break;
        }
    }

    return coord;
}

OBEL_COORD_FUNC bool obel_coord_heap_remove(obel_coord_heap_t *heap, obel_coord_t coord) {
    for (uint32_t i = OBEL_HEAP_ROOT; i < heap->count; i++) {
        if (heap->coords[i].x == coord.x && heap->coords[i].y == coord.y && heap->coords[i].z == coord.z) {
            obel_coord_heap_remove(heap, i);
            return true;
        }
    }

    return false;
}

// MARK: - Coordinate Marching

OBEL_COORD_FUNC uint32_t obel_coord_march(obel_coord_marcher_t *marcher) {
    obel_coord_t *coords = marcher->band.table.coords;

    // Extract the root vertex from the max heap.
    obel_coord_t point_coord = obel_coord_heap_remove(&marcher->vertices, OBEL_HEAP_ROOT);

    // Reset the arrival time to zero.
    uint32_t point_coord_i;
    assert(obel_coord_table_find(&marcher->band.table, point_coord, &point_coord_i));
    coords[point_coord_i].bits &= ~OBEL_COORD_BITS_ARR_TIME;

    // Mark the vertex as alive.
    coords[point_coord_i].bits &= ~OBEL_COORD_BITS_CLOSE;
    coords[point_coord_i].bits |= OBEL_COORD_BITS_ALIVE;

    // Even though the vertex is not considered CLOSE anymore, insert it into the propagation heap
    // so its neighbors will be visited.
    obel_coord_heap_insert(&marcher->propagation, point_coord);

    // Propagate until we have no more CLOSE coordinates to visit.
    obel_coord_propagate(marcher);

    return point_coord_i;
}

OBEL_COORD_FUNC void obel_coord_propagate(obel_coord_marcher_t *marcher) {
    obel_coord_t *coords = marcher->band.table.coords;

    while (marcher->propagation.count > 0) {
        obel_coord_t trial_coord = obel_coord_heap_remove(&marcher->propagation, OBEL_HEAP_ROOT);
        uint16_t trial_arrival_time = trial_coord.bits & OBEL_COORD_BITS_ARR_TIME;

        uint32_t trial_i = 0;
        obel_coord_table_find(&marcher->band.table, trial_coord, &trial_i);
        coords[trial_i].bits &= ~OBEL_COORD_BITS_CLOSE;
        coords[trial_i].bits |= OBEL_COORD_BITS_ALIVE;

        uint32_t neighbor_i = 0;
        obel_coord_t neighbors[6] = {
            obel_coord_make(trial_coord.x + 1, trial_coord.y, trial_coord.z),
            obel_coord_make(trial_coord.x - 1, trial_coord.y, trial_coord.z),
            obel_coord_make(trial_coord.x, trial_coord.y + 1, trial_coord.z),
            obel_coord_make(trial_coord.x, trial_coord.y - 1, trial_coord.z),
            obel_coord_make(trial_coord.x, trial_coord.y, trial_coord.z + 1),
            obel_coord_make(trial_coord.x, trial_coord.y, trial_coord.z - 1)
        };

        for (uint32_t i = 0; i < 6; i++) {
            // Ensure the neighbor exists inside the offset band.
            if (obel_coord_table_find(&marcher->band.table, neighbors[i], &neighbor_i)) {
                if (coords[neighbor_i].bits & OBEL_COORD_BITS_ALIVE) {
                    continue;
                }

                uint16_t neighbor_arrival_time = coords[neighbor_i].bits & OBEL_COORD_BITS_ARR_TIME;

                // If the neighbor's arrival time is less than the trial's arrival time, we have
                // collided with another Voronoi cell, so stop propagation in this "direction".
                if (neighbor_arrival_time < trial_arrival_time) {
                    // The neighbor is now visited by another wave.
                    coords[neighbor_i].bits += OBEL_COORD_BITS_PROP_WAVES_ONE;

                    // The number of propagation waves is three (both bits set), we've reached
                    // another Voronoi vertex, which could become the next point coordinate.
                    if (OBEL_COORD_IS_VORONOI(coords[neighbor_i].bits)) {
                        obel_coord_heap_insert(&marcher->vertices, coords[neighbor_i]);
                    }
                } else {
                    // Increment the arrival time, using the trial coordinate's arrival time.
                    coords[neighbor_i].bits = ((coords[neighbor_i].bits & ~OBEL_COORD_BITS_ARR_TIME) |
                                               ((trial_coord.bits & OBEL_COORD_BITS_ARR_TIME) + 1));

                    if (OBEL_COORD_IS_VORONOI(coords[neighbor_i].bits)) {
                        obel_coord_heap_remove(&marcher->vertices, coords[neighbor_i]);
                    }

                    // The neighbor coordinate has now only been visited by a single propagation
                    // wave.
                    coords[neighbor_i].bits &= ~OBEL_COORD_BITS_PROP_WAVES;
                    coords[neighbor_i].bits |= OBEL_COORD_BITS_PROP_WAVES_ONE;

                    // Insert the neighbor coordinate into the propagation wave.
                    coords[neighbor_i].bits |= OBEL_COORD_BITS_CLOSE;
                    obel_coord_heap_insert(&marcher->propagation, coords[neighbor_i]);
                }
            }
        }
    }
}

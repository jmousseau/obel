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

//
//  coord.h
//  obel
//
//  Created by Jack Mousseau on 10/2/21.
//

#ifndef coord_h
#define coord_h

#include <obel/define.h>
#include <obel/heap.h>
#include <obel/vector.h>

#define OBEL_COORD_FUNC OBEL_OVERLOADABLE

typedef struct obel_coord {
    uint16_t bits, x, y, z;
} obel_coord_t;

typedef struct obel_coord_table {
    uint32_t capacity;
    uint32_t count;
    obel_coord_t *coords;
} obel_coord_table_t;

typedef struct obel_coord_grid {
    obel_vector3_t origin;
    float stride;
    uint16_t width, height, depth;
} obel_coord_grid_t;

typedef struct obel_coord_heap {
    obel_heap_order_t order;
    uint16_t mask;
    uint32_t capacity;
    uint32_t count;
    obel_coord_t *coords;
} obel_coord_heap_t;

// MARK: - Coordinate

#define OBEL_COORD_BITS_OCCUPIED 0x8000

OBEL_COORD_FUNC OBEL_INLINE static obel_coord_t obel_coord_make(uint16_t x, uint16_t y, uint16_t z, uint16_t bits) {
    return (obel_coord_t){ bits, x, y, z };
}

OBEL_COORD_FUNC OBEL_INLINE static obel_coord_t obel_coord_make(uint16_t x, uint16_t y, uint16_t z) {
    return obel_coord_make(x, y, z, 0);
}

// MARK: - Coordinate Table

OBEL_COORD_FUNC uint64_t obel_coord_xyz_hash(obel_coord_t coord);

OBEL_COORD_FUNC bool obel_coord_table_insert(obel_coord_table_t *table, obel_coord_t coord);
OBEL_COORD_FUNC bool obel_coord_table_insert(obel_coord_table_t *table, obel_coord_t coord, uint32_t *i);

OBEL_COORD_FUNC bool obel_coord_table_remove(obel_coord_table_t *table, obel_coord_t coord);
OBEL_COORD_FUNC bool obel_coord_table_remove(obel_coord_table_t *table, obel_coord_t coord, uint32_t *i);

OBEL_COORD_FUNC bool obel_coord_table_find(obel_coord_table_t *table, obel_coord_t coord);
OBEL_COORD_FUNC bool obel_coord_table_find(obel_coord_table_t *table, obel_coord_t coord, uint32_t *i);

// MARK: - Sparse Coordinate Grid

OBEL_COORD_FUNC obel_coord_t obel_coord_grid_bin(float stride, obel_vector3_t origin, obel_vector3_t point);
OBEL_COORD_FUNC obel_coord_t obel_coord_grid_bin(obel_coord_grid_t *grid, obel_vector3_t point);

// MARK: - Coordinate Heap

OBEL_COORD_FUNC uint32_t obel_coord_heap_insert(obel_coord_heap_t *heap, obel_coord_t coord);
OBEL_COORD_FUNC obel_coord_t obel_coord_heap_remove(obel_coord_heap_t *heap, uint32_t i);

#endif // coord_h

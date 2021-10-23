//
//  coord.h
//  obel
//
//  Created by Jack Mousseau on 10/2/21.
//

#ifndef coord_h
#define coord_h

#include <obel/aabb.h>
#include <obel/define.h>
#include <obel/heap.h>
#include <obel/vector.h>

#define OBEL_COORD_FUNC OBEL_OVERLOADABLE

typedef struct obel_coord {
    uint16_t bits, x, y, z;
} obel_coord_t;

typedef struct obel_coord_offset {
    int16_t x, y, z;
} obel_coord_offset_t;

typedef struct obel_coord_neighborhood {
    obel_coord_offset_t *offsets;
    uint32_t count;
} obel_coord_neighborhood_t;

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

typedef struct obel_coord_offset_band {
    float radius;
    obel_coord_grid_t grid;
    obel_coord_table_t table;
} obel_coord_offset_band_t;

typedef struct obel_coord_marcher {
    obel_coord_offset_band_t band;
    obel_coord_heap_t propagation;
    obel_coord_heap_t vertices;
} obel_coord_marcher_t;

// MARK: - Coordinate


#define OBEL_COORD_BITS_OCCUPIED       0x8000
#define OBEL_COORD_BITS_ALIVE          0x4000
#define OBEL_COORD_BITS_CLOSE          0x2000
#define OBEL_COORD_BITS_PROP_WAVES     0x1800
#define OBEL_COORD_BITS_PROP_WAVES_ONE 0x0800
#define OBEL_COORD_BITS_ARR_TIME       0x04ff


#define OBEL_COORD_ADD_PROP_WAVE(bits) \
    ((bits) += 0x0100)

#define OBEL_COORD_IS_VORONOI(bits) \
    (((bits) & OBEL_COORD_BITS_PROP_WAVES) == OBEL_COORD_BITS_PROP_WAVES)

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

OBEL_COORD_FUNC obel_coord_grid_t obel_coord_grid_make(float stride, obel_aabb_t aabb);

OBEL_COORD_FUNC obel_coord_t obel_coord_grid_bin(float stride, obel_vector3_t origin, obel_vector3_t point);
OBEL_COORD_FUNC obel_coord_t obel_coord_grid_bin(obel_coord_grid_t *grid, obel_vector3_t point);

// MARK: - Coordinate Heap

OBEL_COORD_FUNC uint32_t obel_coord_heap_insert(obel_coord_heap_t *heap, obel_coord_t coord);
OBEL_COORD_FUNC obel_coord_t obel_coord_heap_remove(obel_coord_heap_t *heap, uint32_t i);
OBEL_COORD_FUNC bool obel_coord_heap_remove(obel_coord_heap_t *heap, obel_coord_t coord);

// MARK: - Coordinate Marching

OBEL_COORD_FUNC uint32_t obel_coord_march(obel_coord_marcher_t *marcher);
OBEL_COORD_FUNC void obel_coord_propagate(obel_coord_marcher_t *marcher);

#endif // coord_h

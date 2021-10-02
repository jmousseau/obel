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

typedef struct obel_coord {
    uint16_t bits, x, y, z;
} obel_coord_t;

typedef struct obel_coord_heap {
    obel_heap_order_t order;
    uint16_t mask;
    uint32_t capacity;
    uint32_t count;
    obel_coord_t *coords;
} obel_coord_heap_t;

uint32_t obel_coord_heap_insert(obel_coord_heap_t *heap, obel_coord_t coord);
obel_coord_t obel_coord_heap_remove(obel_coord_heap_t *heap, uint32_t i);

#endif // coord_h

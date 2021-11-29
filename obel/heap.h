//
//  heap.h
//  obel
//
//  Created by Jack Mousseau on 10/2/21.
//

#ifndef heap_h
#define heap_h

#include <obel/define.h>

typedef enum : uint32_t {
    obel_heap_order_min,
    obel_heap_order_max
} obel_heap_order_t;

#define OBEL_HEAP_ROOT 1
#define OBEL_HEAP_PARENT(i) ((i) / 2)
#define OBEL_HEAP_LEFT(i) (2 * (i))
#define OBEL_HEAP_RIGHT(i) (2 * (i) + 1)
#define OBEL_HEAP_IS_LEAF(i, count) ((i) > ((count) / 2) && (i) <= (count))

#endif // heap_h

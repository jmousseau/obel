//
//  coord.c
//  obel
//
//  Created by Jack Mousseau on 10/2/21.
//

#include <obel/coord.h>

uint32_t obel_coord_heap_insert(obel_coord_heap_t *heap, obel_coord_t coord) {
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

obel_coord_t obel_coord_heap_remove(obel_coord_heap_t *heap, uint32_t i) {
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

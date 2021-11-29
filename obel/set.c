//
//  set.c
//  obel
//
//  Created by Jack Mousseau on 11/29/21.
//

#include <obel/set.h>

OBEL_SPARSE_FUNC void obel_sparse_set_insert(obel_sparse_set_t *set, uint16_t element) {
    OBEL_ASSERT(element < set->capacity);
    if (!obel_sparse_set_contains(set, element)) {
        uint16_t dense = set->count++;
        set->dense[dense] = element;
        set->sparse[element] = dense;
    }
}

OBEL_SPARSE_FUNC void obel_sparse_set_remove(obel_sparse_set_t *set, uint16_t element) {
    OBEL_ASSERT(element < set->capacity);
    if (obel_sparse_set_contains(set, element)) {
        uint16_t dense = set->sparse[element];
        uint16_t last = set->dense[--set->count];
        set->dense[dense] = last;
        set->sparse[last] = dense;
    }
}

OBEL_SPARSE_FUNC bool obel_sparse_set_contains(obel_sparse_set_t *set, uint16_t element) {
    OBEL_ASSERT(element < set->capacity);
    uint16_t dense = set->sparse[element];
    return (dense < set->count && set->dense[dense] == element);
}

OBEL_SPARSE_FUNC void obel_sparse_set_clear(obel_sparse_set_t *set) {
    set->count = 0;
}

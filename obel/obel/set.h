//
//  set.h
//  obel
//
//  Created by Jack Mousseau on 11/29/21.
//

#ifndef set_h
#define set_h

#include <obel/define.h>

#define OBEL_SPARSE_FUNC

typedef struct obel_sparse_set {
    uint16_t capacity;
    uint16_t count;
    uint16_t *sparse;
    uint16_t *dense;
} obel_sparse_set_t;


OBEL_SPARSE_FUNC void obel_sparse_set_insert(obel_sparse_set_t *set, uint16_t element);
OBEL_SPARSE_FUNC void obel_sparse_set_remove(obel_sparse_set_t *set, uint16_t element);
OBEL_SPARSE_FUNC bool obel_sparse_set_contains(obel_sparse_set_t *set, uint16_t element);
OBEL_SPARSE_FUNC void obel_sparse_set_clear(obel_sparse_set_t *set);

#endif // set_h

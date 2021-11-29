//
//  define.h
//  obel
//
//  Created by Jack Mousseau on 9/21/21.
//

#ifndef define_h
#define define_h

#include <assert.h>
#include <fcntl.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct stat stat_t;

#define OBEL_INLINE __attribute__((__always_inline__))
#define OBEL_OVERLOADABLE __attribute__((__overloadable__))
#define OBEL_CONST __attribute__((__const__))

#define OBEL_ASSERT(x) assert(x)

#define OBEL_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define OBEL_MAX(a, b) (((a) > (b)) ? (a) : (b))

#define OBEL_SAFE_DEREF_ASSIGN(pointer, value) if ((pointer)) *(pointer) = (value)

OBEL_INLINE OBEL_OVERLOADABLE static void obel_swap(uint16_t *a, uint16_t *b) {
    (*a) = (*a) ^ (*b);
    (*b) = (*a) ^ (*b);
    (*a) = (*a) ^ (*b);
}

#endif // define_h

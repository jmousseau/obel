//
//  define.h
//  obel
//
//  Created by Jack Mousseau on 9/21/21.
//

#ifndef base_h
#define base_h

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define OBEL_INLINE __attribute__((__always_inline__))
#define OBEL_OVERLOADABLE __attribute__((__overloadable__))
#define OBEL_CONST __attribute__((__const__))

#define OBEL_ASSERT(x) assert(x)

#define OBEL_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define OBEL_MAX(a, b) (((a) > (b)) ? (a) : (b))

#endif // define_h

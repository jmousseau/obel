//
//  memory.h
//  obel
//
//  Created by Jack Mousseau on 10/16/21.
//

#ifndef memory_h
#define memory_h

#include <obel/define.h>

#define OBEL_MEMORY_FUNC OBEL_OVERLOADABLE
#define OBEL_DEFAULT_ALIGNMENT (2UL * sizeof(uintptr_t))

#define OBEL_KIBIBYTES(n) (1024UL * ((size_t)n))
#define OBEL_MEBIBYTES(n) (1024UL * 1024UL * ((size_t)n))
#define OBEL_GIBIBYTES(n) (1024UL * 1024UL * 1024UL * ((size_t)n))

typedef uint8_t obel_byte_t;

OBEL_MEMORY_FUNC OBEL_INLINE static obel_byte_t *obel_memory_alloc(size_t count);
OBEL_MEMORY_FUNC OBEL_INLINE static obel_byte_t *obel_memory_alloc(size_t count, size_t alignment);
OBEL_MEMORY_FUNC OBEL_INLINE static void obel_memory_free(obel_byte_t *bytes);
OBEL_MEMORY_FUNC OBEL_INLINE static size_t obel_memory_align(obel_byte_t *bytes, size_t alignment);

OBEL_MEMORY_FUNC OBEL_INLINE static obel_byte_t *obel_memory_alloc(size_t count) {
    return obel_memory_alloc(count, OBEL_DEFAULT_ALIGNMENT);
}

OBEL_MEMORY_FUNC OBEL_INLINE static obel_byte_t *obel_memory_alloc(size_t count, size_t alignment) {
    count += obel_memory_align((obel_byte_t *)count, alignment);
    obel_byte_t *bytes = (obel_byte_t *)aligned_alloc(alignment, count);
    memset(bytes, 0, count);
    return bytes;
}

OBEL_MEMORY_FUNC OBEL_INLINE static void obel_memory_free(obel_byte_t *bytes) {
    free(bytes);
}

OBEL_MEMORY_FUNC OBEL_INLINE static size_t obel_memory_align(obel_byte_t *bytes, size_t alignment) {
    size_t remainder = (size_t)bytes & (alignment - 1);
    return remainder ? alignment - remainder : 0;
}

#endif // memory_h

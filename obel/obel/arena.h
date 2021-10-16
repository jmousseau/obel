//
//  arena.h
//  obel
//
//  Created by Jack Mousseau on 10/16/21.
//

#ifndef arena_h
#define arena_h

#include <obel/define.h>
#include <obel/memory.h>

#define OBEL_ARENA_FUNC OBEL_OVERLOADABLE

typedef struct obel_arena {
    size_t capacity;
    size_t count;
    obel_byte_t *bytes;
} obel_arena_t;

OBEL_ARENA_FUNC obel_arena_t *obel_arena_alloc(size_t capacity);
OBEL_ARENA_FUNC void obel_arena_free(obel_arena_t *arena);

OBEL_ARENA_FUNC obel_byte_t *obel_arena_push(obel_arena_t *arena, size_t count);
OBEL_ARENA_FUNC obel_byte_t *obel_arena_push(obel_arena_t *arena, size_t count, size_t alignment);

OBEL_ARENA_FUNC void obel_arena_pop(obel_arena_t *arena, obel_byte_t *bytes);

#endif // arena_h

//
//  arena.c
//  obel
//
//  Created by Jack Mousseau on 10/16/21.
//

#include <obel/arena.h>

OBEL_ARENA_FUNC obel_arena_t *obel_arena_alloc(size_t capacity) {
    obel_arena_t *arena = (obel_arena_t *)obel_memory_alloc(sizeof(obel_arena_t));
    arena->bytes = obel_memory_alloc(capacity);
    arena->capacity = capacity;
    return arena;
}

OBEL_ARENA_FUNC void obel_arena_free(obel_arena_t *arena) {
    free(arena->bytes);
    free(arena);
}

OBEL_ARENA_FUNC obel_byte_t *obel_arena_push(obel_arena_t *arena, size_t count) {
    assert(count <= arena->capacity - arena->count);
    obel_byte_t *bytes = arena->bytes + arena->count;
    arena->count += count;
    return bytes;
}

OBEL_ARENA_FUNC obel_byte_t *obel_arena_push(obel_arena_t *arena, size_t count, size_t alignment) {
    obel_byte_t *bytes = arena->bytes + arena->count;
    size_t padding = obel_memory_align(bytes, alignment);
    assert(count <= arena->capacity - arena->count - padding);
    arena->count += (count + padding);
    return bytes + padding;
}

OBEL_ARENA_FUNC void obel_arena_pop(obel_arena_t *arena, obel_byte_t *bytes) {
    obel_byte_t *start = arena->bytes;
    obel_byte_t *end = arena->bytes + arena->capacity;
    assert(start <= bytes && bytes <= end);
    arena->count = bytes - arena->bytes;
}

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

// MARK: - Pushes

OBEL_ARENA_FUNC obel_coord_t *obel_arena_push_coord(obel_arena_t *arena, uint32_t count) {
    size_t size = sizeof(obel_coord_t);
    return (obel_coord_t *)obel_arena_push(arena, count * size, size);
}

OBEL_ARENA_FUNC obel_coord_offset_t *obel_arena_push_coord_offset(obel_arena_t *arena, uint32_t count) {
    size_t size = sizeof(obel_coord_offset_t);
    return (obel_coord_offset_t *)obel_arena_push(arena, count * size, size);
}

OBEL_COORD_FUNC obel_coord_neighborhood_t obel_coord_neighborhood_push(obel_arena_t *arena, float stride, float radius) {
    int16_t half_side_count = floorf(radius / stride);
    int16_t side_count = 2 * half_side_count;
    int16_t count = side_count * side_count * side_count - 1;

    obel_coord_neighborhood_t neighborhood = {
        .count = 0,
        .offsets = obel_arena_push_coord_offset(arena, count)
    };

    for (int16_t x = -half_side_count; x <= half_side_count; x++) {
        for (int16_t y = -half_side_count; y <= half_side_count; y++) {
            for (int16_t z = -half_side_count; z <= half_side_count; z++) {
                if (x == 0 && y == 0 && z == 0) {
                    continue;
                }

                neighborhood.offsets[neighborhood.count++] = (obel_coord_offset_t){ x, y, z };
            }
        }
    }

    return neighborhood;
}

OBEL_ARENA_FUNC obel_sparse_set_t obel_coord_sparse_set_push(obel_arena_t *arena, uint16_t capacity) {
    size_t size = capacity * sizeof(uint16_t);
    return (obel_sparse_set_t){
        .capacity = capacity,
        .count = 0,
        .sparse = (uint16_t *)obel_arena_push(arena, size),
        .dense = (uint16_t *)obel_arena_push(arena, size)
    };
}

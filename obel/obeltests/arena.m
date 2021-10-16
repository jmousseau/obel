//
//  arena.m
//  obeltests
//
//  Created by Jack Mousseau on 10/16/21.
//

#import <obel/arena.h>
#import <XCTest/XCTest.h>

@interface ObelArenaTests : XCTestCase

@end

@implementation ObelArenaTests

- (void)testAlloc {
    obel_arena_t *arena = obel_arena_alloc(10);
    XCTAssertNotEqual(arena, NULL);
    XCTAssertEqual(arena->capacity, 10);
    XCTAssertEqual(arena->count, 0);
    XCTAssertNotEqual(arena->bytes, NULL);
    obel_arena_free(arena);
}

- (void)testPush {
    obel_arena_t *arena = obel_arena_alloc(64);
    obel_byte_t *bytes = obel_arena_push(arena, 30);
    XCTAssertEqual(arena->count, 30);
    XCTAssertEqual(bytes, arena->bytes);
    bytes = obel_arena_push(arena, 32, OBEL_DEFAULT_ALIGNMENT);
    XCTAssertEqual(arena->count, 64);
    XCTAssertEqual(bytes, arena->bytes + 32);
    obel_arena_free(arena);
}

- (void)testPop {
    obel_arena_t *arena = obel_arena_alloc(64);
    obel_byte_t *bytes = obel_arena_push(arena, 30);
    bytes = obel_arena_push(arena, 32, OBEL_DEFAULT_ALIGNMENT);
    obel_arena_pop(arena, bytes);
    XCTAssertEqual(arena->count, 32);
    obel_arena_free(arena);
}

@end

//
//  coord.m
//  obeltests
//
//  Created by Jack Mousseau on 10/2/21.
//

#import <obel/coord.h>
#import <XCTest/XCTest.h>

@interface ObelCoordTests : XCTestCase

@end

@implementation ObelCoordTests

- (void)testCoordLayout {
    XCTAssertEqual(sizeof(obel_coord_t), sizeof(uint64_t));
}

- (void)testMinHeap {
    obel_coord_t coords[6] = { { .bits = 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    obel_coord_heap_t min_heap = {
        .order = obel_heap_order_min,
        .mask = 0xffff,
        .capacity = 5,
        .count = 0,
        .coords = coords
    };

    XCTAssertEqual(obel_coord_heap_insert(&min_heap, (obel_coord_t){ .bits = 3 }), 1);
    XCTAssertEqual(min_heap.coords[OBEL_HEAP_ROOT].bits, 3);

    XCTAssertEqual(obel_coord_heap_insert(&min_heap, (obel_coord_t){ .bits = 4 }), 2);
    XCTAssertEqual(min_heap.coords[OBEL_HEAP_ROOT].bits, 3);

    XCTAssertEqual(obel_coord_heap_insert(&min_heap, (obel_coord_t){ .bits = 2 }), 1);
    XCTAssertEqual(min_heap.coords[OBEL_HEAP_ROOT].bits, 2);

    XCTAssertEqual(obel_coord_heap_insert(&min_heap, (obel_coord_t){ .bits = 5 }), 4);
    XCTAssertEqual(min_heap.coords[OBEL_HEAP_ROOT].bits, 2);

    XCTAssertEqual(obel_coord_heap_insert(&min_heap, (obel_coord_t){ .bits = 1 }), 1);
    XCTAssertEqual(min_heap.coords[OBEL_HEAP_ROOT].bits, 1);

    XCTAssertEqual(obel_coord_heap_insert(&min_heap, (obel_coord_t){ .bits = 0 }), 0);
    XCTAssertEqual(min_heap.coords[OBEL_HEAP_ROOT].bits, 1);

    XCTAssertEqual(obel_coord_heap_remove(&min_heap, OBEL_HEAP_ROOT).bits, 1);
    XCTAssertEqual(obel_coord_heap_remove(&min_heap, OBEL_HEAP_ROOT).bits, 2);
    XCTAssertEqual(obel_coord_heap_remove(&min_heap, OBEL_HEAP_ROOT).bits, 3);
    XCTAssertEqual(obel_coord_heap_remove(&min_heap, OBEL_HEAP_ROOT).bits, 4);
    XCTAssertEqual(obel_coord_heap_remove(&min_heap, OBEL_HEAP_ROOT).bits, 5);
}

- (void)testMaxHeap {
    obel_coord_t coords[6] = { { .bits = UINT16_MAX }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    obel_coord_heap_t max_heap = {
        .order = obel_heap_order_max,
        .mask = 0xffff,
        .capacity = 5,
        .count = 0,
        .coords = coords
    };

    XCTAssertEqual(obel_coord_heap_insert(&max_heap, (obel_coord_t){ .bits = 1 }), 1);
    XCTAssertEqual(max_heap.coords[OBEL_HEAP_ROOT].bits, 1);

    XCTAssertEqual(obel_coord_heap_insert(&max_heap, (obel_coord_t){ .bits = 2 }), 1);
    XCTAssertEqual(max_heap.coords[OBEL_HEAP_ROOT].bits, 2);

    XCTAssertEqual(obel_coord_heap_insert(&max_heap, (obel_coord_t){ .bits = 4 }), 1);
    XCTAssertEqual(max_heap.coords[OBEL_HEAP_ROOT].bits, 4);

    XCTAssertEqual(obel_coord_heap_insert(&max_heap, (obel_coord_t){ .bits = 5 }), 1);
    XCTAssertEqual(max_heap.coords[OBEL_HEAP_ROOT].bits, 5);

    XCTAssertEqual(obel_coord_heap_insert(&max_heap, (obel_coord_t){ .bits = 3 }), 5);
    XCTAssertEqual(max_heap.coords[OBEL_HEAP_ROOT].bits, 5);

    XCTAssertEqual(obel_coord_heap_insert(&max_heap, (obel_coord_t){ .bits = 6 }), 0);
    XCTAssertEqual(max_heap.coords[OBEL_HEAP_ROOT].bits, 5);

    XCTAssertEqual(obel_coord_heap_remove(&max_heap, OBEL_HEAP_ROOT).bits, 5);
    XCTAssertEqual(obel_coord_heap_remove(&max_heap, OBEL_HEAP_ROOT).bits, 4);
    XCTAssertEqual(obel_coord_heap_remove(&max_heap, OBEL_HEAP_ROOT).bits, 3);
    XCTAssertEqual(obel_coord_heap_remove(&max_heap, OBEL_HEAP_ROOT).bits, 2);
    XCTAssertEqual(obel_coord_heap_remove(&max_heap, OBEL_HEAP_ROOT).bits, 1);
}

@end

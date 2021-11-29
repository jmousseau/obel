//
//  heap.m
//  obeltests
//
//  Created by Jack Mousseau on 10/2/21.
//

#import <obel/heap.h>
#import <XCTest/XCTest.h>

@interface ObelHeapTests : XCTestCase

@end

@implementation ObelHeapTests

- (void)testHeapOrderLayout {
    XCTAssertEqual(sizeof(obel_heap_order_t), sizeof(uint32_t));
}

- (void)testHeapRoot {
    XCTAssertEqual(OBEL_HEAP_ROOT, 1);
}

- (void)testHeapLeft {
    XCTAssertEqual(OBEL_HEAP_LEFT(1), 2);
    XCTAssertEqual(OBEL_HEAP_LEFT(2), 4);
    XCTAssertEqual(OBEL_HEAP_LEFT(3), 6);
    XCTAssertEqual(OBEL_HEAP_LEFT(4), 8);
}

- (void)testHeapRight {
    XCTAssertEqual(OBEL_HEAP_RIGHT(1), 3);
    XCTAssertEqual(OBEL_HEAP_RIGHT(2), 5);
    XCTAssertEqual(OBEL_HEAP_RIGHT(3), 7);
    XCTAssertEqual(OBEL_HEAP_RIGHT(4), 9);
}

- (void)testHeapIsLeaf {
    XCTAssertTrue(OBEL_HEAP_IS_LEAF(1, 1));
    XCTAssertFalse(OBEL_HEAP_IS_LEAF(1, 2));
    XCTAssertTrue(OBEL_HEAP_IS_LEAF(2, 3));
    XCTAssertTrue(OBEL_HEAP_IS_LEAF(2, 3));
    XCTAssertFalse(OBEL_HEAP_IS_LEAF(4, 3));
}

@end

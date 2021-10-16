//
//  memory.m
//  obeltests
//
//  Created by Jack Mousseau on 10/16/21.
//

#import <obel/memory.h>
#import <XCTest/XCTest.h>

@interface ObelMemoryTests : XCTestCase

@end

@implementation ObelMemoryTests

- (void)testDefaultAlignment {
    XCTAssertEqual(OBEL_DEFAULT_ALIGNMENT, 2UL * sizeof(uintptr_t));
}

- (void)testByteCounts {
    XCTAssertEqual(OBEL_KIBIBYTES(4), 4UL * 1024UL);
    XCTAssertEqual(OBEL_MEBIBYTES(5), 5UL * 1024UL * 1024UL);
    XCTAssertEqual(OBEL_GIBIBYTES(6), 6UL * 1024UL * 1024UL * 1024UL);
}

- (void)testByteSize {
    XCTAssertEqual(sizeof(obel_byte_t), sizeof(uint8_t));
}

- (void)testAlloc {
    {
        obel_byte_t *bytes = obel_memory_alloc(34);
        XCTAssertEqual((size_t)bytes % OBEL_DEFAULT_ALIGNMENT, 0);
        for (size_t i = 0; i < 48; i++) {
            XCTAssertEqual(*(bytes + i), 0);
        }
        obel_memory_free(bytes);
    } {
        obel_byte_t *bytes = obel_memory_alloc(34, 32);
        XCTAssertEqual((size_t)bytes % 32, 0);
        for (size_t i = 0; i < 64; i++) {
            XCTAssertEqual(*(bytes + i), 0);
        }
        obel_memory_free(bytes);
    }
}

- (void)testAlign {
    XCTAssertEqual(obel_memory_align((obel_byte_t *)7, 64), 57);
    XCTAssertEqual(obel_memory_align((obel_byte_t *)7, 32), 25);
    XCTAssertEqual(obel_memory_align((obel_byte_t *)7, 16),  9);
    XCTAssertEqual(obel_memory_align((obel_byte_t *)7,  8),  1);
    XCTAssertEqual(obel_memory_align((obel_byte_t *)8,  8),  0);
}

@end

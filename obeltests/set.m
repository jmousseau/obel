//
//  set.m
//  obeltests
//
//  Created by Jack Mousseau on 11/29/21.
//

#import <obel/set.h>
#import <XCTest/XCTest.h>

@interface ObelSetTests : XCTestCase

@end

@implementation ObelSetTests

- (void)testSparseInsert {
    uint16_t sparse[4] = { 0, 1, 0, 0 };
    uint16_t dense[4] = { 3, 1, 0, 0};

    obel_sparse_set_t set = {
        .capacity = 4,
        .count = 2,
        .sparse = sparse,
        .dense = dense
    };

    obel_sparse_set_insert(&set, 0);
    XCTAssertEqual(set.count, 3);
    XCTAssertEqual(set.dense[2], 0);
    XCTAssertEqual(set.sparse[0], 2);

    obel_sparse_set_insert(&set, 1);
    XCTAssertEqual(set.count, 3);
}

- (void)testSparseRemove {
    uint16_t sparse[4] = { 0, 1, 0, 0 };
    uint16_t dense[4] = { 3, 1, 0, 0};

    obel_sparse_set_t set = {
        .capacity = 4,
        .count = 2,
        .sparse = sparse,
        .dense = dense
    };

    obel_sparse_set_remove(&set, 3);
    XCTAssertEqual(set.count, 1);
    XCTAssertEqual(set.dense[0], 1);
    XCTAssertEqual(set.sparse[1], 0);

    obel_sparse_set_remove(&set, 3);
    XCTAssertEqual(set.count, 1);

    obel_sparse_set_remove(&set, 2);
    XCTAssertEqual(set.count, 1);
}

- (void)testSparseContains {
    uint16_t sparse[4] = { 0, 1, 0, 0 };
    uint16_t dense[4] = { 3, 1, 0, 0};

    obel_sparse_set_t set = {
        .capacity = 4,
        .count = 2,
        .sparse = sparse,
        .dense = dense
    };

    XCTAssertFalse(obel_sparse_set_contains(&set, 0));
    XCTAssertTrue(obel_sparse_set_contains(&set, 1));
    XCTAssertFalse(obel_sparse_set_contains(&set, 2));
    XCTAssertTrue(obel_sparse_set_contains(&set, 3));
}

- (void)testSparseSetClear {
    uint16_t sparse[4] = { 0, 1, 0, 0 };
    uint16_t dense[4] = { 3, 1, 0, 0};

    obel_sparse_set_t set = {
        .capacity = 4,
        .count = 2,
        .sparse = sparse,
        .dense = dense
    };

    obel_sparse_set_clear(&set);
    XCTAssertEqual(set.count, 0);
    XCTAssertFalse(obel_sparse_set_contains(&set, 0));
    XCTAssertFalse(obel_sparse_set_contains(&set, 1));
    XCTAssertFalse(obel_sparse_set_contains(&set, 2));
    XCTAssertFalse(obel_sparse_set_contains(&set, 3));
}

@end

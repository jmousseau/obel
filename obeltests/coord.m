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

- (void)testCoordTable {
    const uint32_t capacity = 5;
    obel_coord_t coords[capacity] = { 0 };
    obel_coord_table_t table = {
        .count = 0,
        .capacity = capacity,
        .coords = coords
    };

    obel_coord_t a = obel_coord_make(1, 2, 3);
    obel_coord_t b = obel_coord_make(2, 3, 4);
    obel_coord_t c = obel_coord_make(3, 4, 5);
    obel_coord_t d = obel_coord_make(4, 5, 6);
    obel_coord_t e = obel_coord_make(5, 6, 7);
    obel_coord_t f = obel_coord_make(6, 7, 8);

    {
        uint32_t upper = (capacity + 1);
        uint32_t i_a, i_b, i_c, i_d, i_e, i_f = upper;
        XCTAssertTrue(obel_coord_table_insert(&table, a, &i_a) && i_a < upper && table.count == 1);
        XCTAssertTrue(obel_coord_table_insert(&table, b, &i_b) && i_b < upper && table.count == 2);
        XCTAssertTrue(obel_coord_table_insert(&table, c, &i_c) && i_c < upper && table.count == 3);
        XCTAssertTrue(obel_coord_table_insert(&table, d, &i_d) && i_d < upper && table.count == 4);
        XCTAssertTrue(obel_coord_table_insert(&table, e, &i_e) && i_e < upper && table.count == 5);
        XCTAssertTrue(!obel_coord_table_insert(&table, f, &i_f) && i_f == upper && table.count == 5);

        uint32_t j_a, j_b, j_c, j_d, j_e, j_f = upper;
        XCTAssertTrue(obel_coord_table_find(&table, a, &j_a) && j_a == i_a && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, b, &j_b) && j_b == i_b && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, c, &j_c) && j_c == i_c && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, d, &j_d) && j_d == i_d && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, e, &j_e) && j_e == i_e && table.count == 5);
        XCTAssertTrue(!obel_coord_table_find(&table, f, &j_f) && j_f == upper && table.count == 5);

        uint32_t k_a, k_b, k_c, k_d, k_e, k_f = upper;
        XCTAssertTrue(obel_coord_table_remove(&table, a, &k_a) && k_a == i_a && table.count == 4);
        XCTAssertTrue(obel_coord_table_remove(&table, b, &k_b) && k_b == i_b && table.count == 3);
        XCTAssertTrue(obel_coord_table_remove(&table, c, &k_c) && k_c == i_c && table.count == 2);
        XCTAssertTrue(obel_coord_table_remove(&table, d, &k_d) && k_d == i_d && table.count == 1);
        XCTAssertTrue(obel_coord_table_remove(&table, e, &k_e) && k_e == i_e && table.count == 0);
        XCTAssertTrue(!obel_coord_table_remove(&table, f, &k_f) && k_f == upper && table.count == 0);
    }

    {
        XCTAssertTrue(obel_coord_table_insert(&table, a) && table.count == 1);
        XCTAssertTrue(obel_coord_table_insert(&table, b) && table.count == 2);
        XCTAssertTrue(obel_coord_table_insert(&table, c) && table.count == 3);
        XCTAssertTrue(obel_coord_table_insert(&table, d) && table.count == 4);
        XCTAssertTrue(obel_coord_table_insert(&table, e) && table.count == 5);
        XCTAssertTrue(!obel_coord_table_insert(&table, f) && table.count == 5);

        XCTAssertTrue(obel_coord_table_find(&table, a) && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, b) && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, c) && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, d) && table.count == 5);
        XCTAssertTrue(obel_coord_table_find(&table, e) && table.count == 5);
        XCTAssertTrue(!obel_coord_table_find(&table, f) && table.count == 5);

        XCTAssertTrue(obel_coord_table_remove(&table, a) && table.count == 4);
        XCTAssertTrue(obel_coord_table_remove(&table, b) && table.count == 3);
        XCTAssertTrue(obel_coord_table_remove(&table, c) && table.count == 2);
        XCTAssertTrue(obel_coord_table_remove(&table, d) && table.count == 1);
        XCTAssertTrue(obel_coord_table_remove(&table, e) && table.count == 0);
        XCTAssertTrue(!obel_coord_table_remove(&table, f) && table.count == 0);
    }
}

- (void)testCoordGridBin {
    float stride = 1.5;
    obel_vector3_t origin = obel_vector3_make(1.f, 2.f, 3.f);
    obel_vector3_t point = obel_vector3_make(9.f, 5.f, 4.f);

    {
        obel_coord_t coord = obel_coord_grid_bin(stride, origin, point);
        XCTAssertEqual(coord.x, 5);
        XCTAssertEqual(coord.y, 2);
        XCTAssertEqual(coord.z, 0);
    }

    {
        obel_coord_grid_t grid = {
            .origin = origin,
            .stride = stride
        };

        obel_coord_t coord = obel_coord_grid_bin(&grid, point);
        XCTAssertEqual(coord.x, 5);
        XCTAssertEqual(coord.y, 2);
        XCTAssertEqual(coord.z, 0);
    }
}

@end

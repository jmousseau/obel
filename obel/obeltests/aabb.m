//
//  aabb.m
//  obeltests
//
//  Created by Jack Mousseau on 9/22/21.
//

#import <obel/aabb.h>
#import <XCTest/XCTest.h>

@interface ObelAABBTests : XCTestCase

@end

@implementation ObelAABBTests

- (void)testAABBLayout {
    obel_aabb_t a = { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f };
    XCTAssertEqual(a.min.x, 1.f);
    XCTAssertEqual(a.min.y, 2.f);
    XCTAssertEqual(a.min.z, 3.f);
    XCTAssertEqual(a.max.x, 4.f);
    XCTAssertEqual(a.max.y, 5.f);
    XCTAssertEqual(a.max.z, 6.f);
}

- (void)testAABBVolume {
    obel_aabb_t a = { 1.f, 3.f, 2.f, 4.f, 5.f, 6.f };
    XCTAssertEqual(obel_aabb_volume(a), 24.f);
}

- (void)testAABBIntersectionOverUnion {
    obel_aabb_t a = { 1.f, 2.f, 3.f, 11.f, 22.f, 33.f };
    obel_aabb_t b = { 6.f, 7.f, 8.f, 16.f, 27.f, 38.f };
    XCTAssertEqual(obel_aabb_iou(a, b), 1875.f / (12000.f - 1875.f));
}

- (void)testAABBUnion {
    obel_aabb_t a = { 1.f, 2.f, 3.f, 11.f, 22.f, 33.f };
    obel_aabb_t b = { 6.f, 7.f, 8.f, 16.f, 27.f, 38.f };
    obel_aabb_t aabb = obel_aabb_union(a, b);
    XCTAssertEqual(aabb.min.x,  1.f);
    XCTAssertEqual(aabb.min.y,  2.f);
    XCTAssertEqual(aabb.min.z,  3.f);
    XCTAssertEqual(aabb.max.x, 16.f);
    XCTAssertEqual(aabb.max.y, 27.f);
    XCTAssertEqual(aabb.max.z, 38.f);
}

@end

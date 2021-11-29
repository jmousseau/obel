//
//  cloud.m
//  obeltests
//
//  Created by Jack Mousseau on 9/23/21.
//

#import <obel/cloud.h>
#import <XCTest/XCTest.h>

@interface ObelCloudTests : XCTestCase

@end

@implementation ObelCloudTests

- (void)testCloudAABB {
    obel_vector3_t points[3] = { { 1.f, 2.f, 3.f }, { 2.f, 3.f, 4.f }, { 3.f, 4.f, 5.f } };
    obel_cloud_t cloud = { .capacity = 3, .count = 3, .points = points };
    obel_aabb_t aabb = obel_cloud_aabb(&cloud);
    XCTAssertEqual(aabb.min.x, 1.f);
    XCTAssertEqual(aabb.min.y, 2.f);
    XCTAssertEqual(aabb.min.z, 3.f);
    XCTAssertEqual(aabb.max.x, 3.f);
    XCTAssertEqual(aabb.max.y, 4.f);
    XCTAssertEqual(aabb.max.z, 5.f);
}

- (void)testCloudStat {
    obel_vector3_t points[3] = { { 1.f, 2.f, 3.f }, { 2.f, 3.f, 4.f }, { 3.f, 4.f, 5.f } };
    obel_cloud_t cloud = { .capacity = 3, .count = 3, .points = points };
    obel_cloud_stat_t stat = obel_cloud_stat(&cloud);
    XCTAssertEqual(stat.aabb.min.x, 1.f);
    XCTAssertEqual(stat.aabb.min.y, 2.f);
    XCTAssertEqual(stat.aabb.min.z, 3.f);
    XCTAssertEqual(stat.aabb.max.x, 3.f);
    XCTAssertEqual(stat.aabb.max.y, 4.f);
    XCTAssertEqual(stat.aabb.max.z, 5.f);
    XCTAssertEqual(stat.center_of_mass.x, 2.f);
    XCTAssertEqual(stat.center_of_mass.y, 3.f);
    XCTAssertEqual(stat.center_of_mass.z, 4.f);
}

- (void)testCloudL2HistogramFit {
    obel_aabb_t aabb = { .min = { 1.f, 2.f, 3.f }, .max = { 3.f, 4.f, 5.f } };
    obel_histogram_t histogram = { .count = 10 };
    obel_cloud_l2_histogram_fit(aabb, &histogram);
    XCTAssertEqualWithAccuracy(histogram.stride, 0.34641f, 1e-4f);
}

- (void)testCloudL2HistogramBin {
    obel_vector3_t points[4] = { { 1.f, 2.f, 3.f }, { 2.f, 3.f, 4.f }, { 3.f, 4.f, 5.f }, { 5.f, 6.f, 7.f } };
    obel_cloud_t cloud = { .capacity = 3, .count = 4, .points = points };
    int32_t bins[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    {
        obel_histogram_t histogram = { .offset = 0.f, .stride = 0.5f, .count = 100, .bins = bins };
        obel_cloud_l2_histogram_bin(&cloud, 4, &histogram);
        XCTAssertEqual(histogram.samples, 4);
    } {
        obel_histogram_t histogram = { .offset = 0.f, .stride = 0.5f, .count = 100, .bins = bins };
        obel_cloud_l2_histogram_bin(&cloud, 10, &histogram);
        XCTAssertEqual(histogram.samples, 6);
    }
}

@end

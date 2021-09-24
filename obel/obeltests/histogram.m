//
//  histogram.m
//  obeltests
//
//  Created by Jack Mousseau on 9/23/21.
//

#import <obel/histogram.h>
#import <XCTest/XCTest.h>

@interface ObelHistogramTests : XCTestCase

@end

@implementation ObelHistogramTests

- (void)testHistogramBin {
    {
        XCTAssertEqual(obel_histogram_bin(0.f, 4.f, 28.f), 7);
        XCTAssertEqual(obel_histogram_bin(1.f, 4.f, 28.f), 6);
    }

    {
        obel_histogram_t histogram = { .offset = 0.f, .stride = 4.f };
        XCTAssertEqual(obel_histogram_bin(&histogram, 28.f), 7);
        histogram.offset = 1.f;
        XCTAssertEqual(obel_histogram_bin(&histogram, 28.f), 6);
    }
}

- (void)testHistogramDistance {
    int32_t bins_a[4] = { 5, 2, 6, 4 };
    int32_t bins_b[4] = { 1, 7, 3, 8 };
    obel_histogram_t histogram_a = { .offset = 0.f, .stride = 1.f, .count = 4, .bins = bins_a };
    obel_histogram_t histogram_b = { .offset = 0.f, .stride = 1.f, .count = 4, .bins = bins_b };
    XCTAssertEqual(obel_histogram_distance(&histogram_a, &histogram_b), 16);
}

@end

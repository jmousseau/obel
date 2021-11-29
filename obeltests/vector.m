//
//  vector.m
//  obeltests
//
//  Created by Jack Mousseau on 9/21/21.
//

#import <XCTest/XCTest.h>
#import <obel/vector.h>

@interface ObelVectorTests : XCTestCase

@end

@implementation ObelVectorTests

- (void)testVector3Layout {
    obel_vector3_t vector = { 1.f, 2.f, 3.f };
    XCTAssertEqual(vector.elements[0], 1.f);
    XCTAssertEqual(vector.elements[1], 2.f);
    XCTAssertEqual(vector.elements[2], 3.f);
    XCTAssertEqual(vector.x, 1.f);
    XCTAssertEqual(vector.y, 2.f);
    XCTAssertEqual(vector.z, 3.f);
    XCTAssertEqual(vector.r, 1.f);
    XCTAssertEqual(vector.g, 2.f);
    XCTAssertEqual(vector.b, 3.f);
}

- (void)testVector4Layout {
    obel_vector4_t vector = { 1.f, 2.f, 3.f, 4.f };
    XCTAssertEqual(vector.elements[0], 1.f);
    XCTAssertEqual(vector.elements[1], 2.f);
    XCTAssertEqual(vector.elements[2], 3.f);
    XCTAssertEqual(vector.elements[3], 4.f);
    XCTAssertEqual(vector.x, 1.f);
    XCTAssertEqual(vector.y, 2.f);
    XCTAssertEqual(vector.z, 3.f);
    XCTAssertEqual(vector.w, 4.f);
    XCTAssertEqual(vector.r, 1.f);
    XCTAssertEqual(vector.g, 2.f);
    XCTAssertEqual(vector.b, 3.f);
    XCTAssertEqual(vector.a, 4.f);
    XCTAssertEqual(vector.xyz.x, 1.f);
    XCTAssertEqual(vector.xyz.y, 2.f);
    XCTAssertEqual(vector.xyz.z, 3.f);
    XCTAssertEqual(vector.rgb.r, 1.f);
    XCTAssertEqual(vector.rgb.g, 2.f);
    XCTAssertEqual(vector.rgb.b, 3.f);
}

// MARK: - Geometry

- (void)testVectorCross {
    obel_vector3_t a = { 1.f, 3.f, 2.f };
    obel_vector3_t b = { 4.f, 5.f, 6.f };
    obel_vector3_t vector = obel_vector_cross(a, b);
    XCTAssertEqual(vector.x,  8.f);
    XCTAssertEqual(vector.y,  2.f);
    XCTAssertEqual(vector.z, -7.f);
}

- (void)testVectorDistance {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t b = { 6.f, 5.f, 4.f };
        XCTAssertEqualWithAccuracy(obel_vector_distance(a, b), 5.9160f, 1e-4f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t b = { 8.f, 7.f, 6.f, 5.f };
        XCTAssertEqualWithAccuracy(obel_vector_distance(a, b), 9.1651f, 1e-4f);
    }
}

- (void)testVectorDot {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t b = { 6.f, 5.f, 4.f };
        XCTAssertEqual(obel_vector_dot(a, b), 28.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t b = { 8.f, 7.f, 6.f, 5.f };
        XCTAssertEqual(obel_vector_dot(a, b), 60.f);
    }
}

- (void)testVectorLength {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        XCTAssertEqualWithAccuracy(obel_vector_length(a), 3.7416f, 1e-4);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        XCTAssertEqualWithAccuracy(obel_vector_length(a), 5.4772f, 1e-4);
    }
}

// MARK: - Logic

- (void)testVectorMin {
    {
        obel_vector3_t a = { 1.f, 5.f, 3.f };
        obel_vector3_t b = { 4.f, 2.f, 6.f };
        obel_vector3_t vector = obel_vector_min(a, b);
        XCTAssertEqual(vector.x, 1.f);
        XCTAssertEqual(vector.y, 2.f);
        XCTAssertEqual(vector.z, 3.f);
    } {
        obel_vector4_t a = { 1.f, 6.f, 7.f, 4.f };
        obel_vector4_t b = { 5.f, 2.f, 3.f, 8.f };
        obel_vector4_t vector = obel_vector_min(a, b);
        XCTAssertEqual(vector.x, 1.f);
        XCTAssertEqual(vector.y, 2.f);
        XCTAssertEqual(vector.z, 3.f);
        XCTAssertEqual(vector.w, 4.f);
    }
}

- (void)testVectorMax {
    {
        obel_vector3_t a = { 1.f, 5.f, 3.f };
        obel_vector3_t b = { 4.f, 2.f, 6.f };
        obel_vector3_t vector = obel_vector_max(a, b);
        XCTAssertEqual(vector.x, 4.f);
        XCTAssertEqual(vector.y, 5.f);
        XCTAssertEqual(vector.z, 6.f);
    } {
        obel_vector4_t a = { 1.f, 6.f, 7.f, 4.f };
        obel_vector4_t b = { 5.f, 2.f, 3.f, 8.f };
        obel_vector4_t vector = obel_vector_max(a, b);
        XCTAssertEqual(vector.x, 5.f);
        XCTAssertEqual(vector.y, 6.f);
        XCTAssertEqual(vector.z, 7.f);
        XCTAssertEqual(vector.w, 8.f);
    }
}

// MARK: - Math

- (void)testVectorAdd {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t b = { 4.f, 5.f, 6.f };
        obel_vector3_t vector = obel_vector_add(a, b);
        XCTAssertEqual(vector.x, 5.f);
        XCTAssertEqual(vector.y, 7.f);
        XCTAssertEqual(vector.z, 9.f);
    } {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t vector = obel_vector_add(a, 4.f);
        XCTAssertEqual(vector.x, 5.f);
        XCTAssertEqual(vector.y, 6.f);
        XCTAssertEqual(vector.z, 7.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t b = { 5.f, 6.f, 7.f, 8.f };
        obel_vector4_t vector = obel_vector_add(a, b);
        XCTAssertEqual(vector.x,  6.f);
        XCTAssertEqual(vector.y,  8.f);
        XCTAssertEqual(vector.z, 10.f);
        XCTAssertEqual(vector.w, 12.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t vector = obel_vector_add(a, 5.f);
        XCTAssertEqual(vector.x, 6.f);
        XCTAssertEqual(vector.y, 7.f);
        XCTAssertEqual(vector.z, 8.f);
        XCTAssertEqual(vector.w, 9.f);
    }
}

- (void)testVectorSubtract {
    {
        obel_vector3_t a = { 1.f, 5.f, 3.f };
        obel_vector3_t b = { 6.f, 2.f, 4.f };
        obel_vector3_t vector = obel_vector_sub(a, b);
        XCTAssertEqual(vector.x, -5.f);
        XCTAssertEqual(vector.y,  3.f);
        XCTAssertEqual(vector.z, -1.f);
    } {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t vector = obel_vector_sub(a, 2.f);
        XCTAssertEqual(vector.x, -1.f);
        XCTAssertEqual(vector.y,  0.f);
        XCTAssertEqual(vector.z,  1.f);
    } {
        obel_vector4_t a = { 2.f, 6.f, 4.f, 8.f };
        obel_vector4_t b = { 7.f, 1.f, 3.f, 5.f };
        obel_vector4_t vector = obel_vector_sub(a, b);
        XCTAssertEqual(vector.x, -5.f);
        XCTAssertEqual(vector.y,  5.f);
        XCTAssertEqual(vector.z,  1.f);
        XCTAssertEqual(vector.w,  3.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t vector = obel_vector_sub(a, 3.f);
        XCTAssertEqual(vector.x, -2.f);
        XCTAssertEqual(vector.y, -1.f);
        XCTAssertEqual(vector.z,  0.f);
        XCTAssertEqual(vector.w,  1.f);
    }
}

- (void)testVectorMultiply {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t b = { 4.f, 5.f, 6.f };
        obel_vector3_t vector = obel_vector_mul(a, b);
        XCTAssertEqual(vector.x,  4.f);
        XCTAssertEqual(vector.y, 10.f);
        XCTAssertEqual(vector.z, 18.f);
    } {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t vector = obel_vector_mul(a, 2.f);
        XCTAssertEqual(vector.x, 2.f);
        XCTAssertEqual(vector.y, 4.f);
        XCTAssertEqual(vector.z, 6.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t b = { 5.f, 6.f, 7.f, 8.f };
        obel_vector4_t vector = obel_vector_mul(a, b);
        XCTAssertEqual(vector.x,  5.f);
        XCTAssertEqual(vector.y, 12.f);
        XCTAssertEqual(vector.z, 21.f);
        XCTAssertEqual(vector.w, 32.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t vector = obel_vector_mul(a, 3.f);
        XCTAssertEqual(vector.x,  3.f);
        XCTAssertEqual(vector.y,  6.f);
        XCTAssertEqual(vector.z,  9.f);
        XCTAssertEqual(vector.w, 12.f);
    }
}

- (void)testVectorDivide {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t b = { 4.f, 5.f, 6.f };
        obel_vector3_t vector = obel_vector_div(a, b);
        XCTAssertEqual(vector.x, 1.f / 4.f);
        XCTAssertEqual(vector.y, 2.f / 5.f);
        XCTAssertEqual(vector.z, 3.f / 6.f);
    } {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        obel_vector3_t vector = obel_vector_div(a, 2.f);
        XCTAssertEqual(vector.x, 1.f / 2.f);
        XCTAssertEqual(vector.y, 2.f / 2.f);
        XCTAssertEqual(vector.z, 3.f / 2.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t b = { 5.f, 6.f, 7.f, 8.f };
        obel_vector4_t vector = obel_vector_div(a, b);
        XCTAssertEqual(vector.x, 1.f / 5.f);
        XCTAssertEqual(vector.y, 2.f / 6.f);
        XCTAssertEqual(vector.z, 3.f / 7.f);
        XCTAssertEqual(vector.w, 4.f / 8.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4.f };
        obel_vector4_t vector = obel_vector_div(a, 3.f);
        XCTAssertEqual(vector.x, 1.f / 3.f);
        XCTAssertEqual(vector.y, 2.f / 3.f);
        XCTAssertEqual(vector.z, 3.f / 3.f);
        XCTAssertEqual(vector.w, 4.f / 3.f);
    }
}

#pragma mark - Reduction

- (void)testVectorReduceAdd {
    {
        obel_vector3_t a = { 1.f, 2.f, 3.f };
        XCTAssertEqual(obel_vector_reduce_add(a), 6.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 3.f, 4 };
        XCTAssertEqual(obel_vector_reduce_add(a), 10.f);
    }
}

- (void)testVectorReduceMultiply {
    {
        obel_vector3_t a = { 2.f, 3.f, 4.f };
        XCTAssertEqual(obel_vector_reduce_mul(a), 24.f);
    } {
        obel_vector4_t a = { 2.f, 3.f, 4.f, 5.f };
        XCTAssertEqual(obel_vector_reduce_mul(a), 120.f);
    }
}

- (void)testVectorReduceMin {
    {
        obel_vector3_t a = { 2.f, 1.f, 3.f };
        XCTAssertEqual(obel_vector_reduce_min(a), 1.f);
    } {
        obel_vector4_t a = { 3.f, 2.f, 1.f, 4.f };
        XCTAssertEqual(obel_vector_reduce_min(a), 1.f);
    }
}

- (void)testVectorReduceMax {
    {
        obel_vector3_t a = { 1.f, 3.f, 2.f };
        XCTAssertEqual(obel_vector_reduce_max(a), 3.f);
    } {
        obel_vector4_t a = { 1.f, 2.f, 4.f, 3.f };
        XCTAssertEqual(obel_vector_reduce_max(a), 4.f);
    }
}

@end

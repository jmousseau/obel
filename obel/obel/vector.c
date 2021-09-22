//
//  vector.c
//  obel
//
//  Created by Jack Mousseau on 9/21/21.
//

#include <obel/vector.h>

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

// MARK: - Geometry

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_cross(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

OBEL_VECTOR_FUNC float obel_vector_distance(obel_vector3_t a, obel_vector3_t b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    return sqrtf(x * x + y * y + z * z);
}

OBEL_VECTOR_FUNC float obel_vector_distance(obel_vector4_t a, obel_vector4_t b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    float w = a.w - b.w;
    return sqrtf(x * x + y * y + z * z + w * w);
}

OBEL_VECTOR_FUNC float obel_vector_dot(obel_vector3_t a, obel_vector3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

OBEL_VECTOR_FUNC float obel_vector_dot(obel_vector4_t a, obel_vector4_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

OBEL_VECTOR_FUNC float obel_vector_length(obel_vector3_t a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

OBEL_VECTOR_FUNC float obel_vector_length(obel_vector4_t a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

// MARK: - Logic

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_min(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z) };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_min(obel_vector4_t a, obel_vector4_t b) {
    return (obel_vector4_t){ MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z), MIN(a.w, b.w) };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_max(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z) };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_max(obel_vector4_t a, obel_vector4_t b) {
    return (obel_vector4_t){ MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z), MAX(a.w, b.w) };
}

// MARK: - Math

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_add(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ a.x + b.x, a.y + b.y, a.z + b.z };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_add(obel_vector3_t a, float b) {
    return (obel_vector3_t){ a.x + b, a.y + b, a.z + b };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_add(obel_vector4_t a, obel_vector4_t b) {
    return (obel_vector4_t){ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_add(obel_vector4_t a, float b) {
    return (obel_vector4_t){ a.x + b, a.y + b, a.z + b, a.w + b };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_sub(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ a.x - b.x, a.y - b.y, a.z - b.z };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_sub(obel_vector3_t a, float b) {
    return (obel_vector3_t){ a.x - b, a.y - b, a.z - b };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_sub(obel_vector4_t a, obel_vector4_t b) {
    return (obel_vector4_t){ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_sub(obel_vector4_t a, float b) {
    return (obel_vector4_t){ a.x - b, a.y - b, a.z - b, a.w - b };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_mul(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ a.x * b.x, a.y * b.y, a.z * b.z };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_mul(obel_vector3_t a, float b) {
    return (obel_vector3_t){ a.x * b, a.y * b, a.z * b };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_mul(obel_vector4_t a, obel_vector4_t b) {
    return (obel_vector4_t){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_mul(obel_vector4_t a, float b) {
    return (obel_vector4_t){ a.x * b, a.y * b, a.z * b, a.w * b };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_div(obel_vector3_t a, obel_vector3_t b) {
    return (obel_vector3_t){ a.x / b.x, a.y / b.y, a.z / b.z };
}

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_div(obel_vector3_t a, float b) {
    return (obel_vector3_t){ a.x / b, a.y / b, a.z / b };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_div(obel_vector4_t a, obel_vector4_t b) {
    return (obel_vector4_t){ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

OBEL_VECTOR_FUNC obel_vector4_t obel_vector_div(obel_vector4_t a, float b) {
    return (obel_vector4_t){ a.x / b, a.y / b, a.z / b, a.w / b };
}

// MARK: - Reduction

OBEL_VECTOR_FUNC float obel_vector_reduce_add(obel_vector3_t a) {
    return a.x + a.y + a.z;
}

OBEL_VECTOR_FUNC float obel_vector_reduce_add(obel_vector4_t a) {
    return a.x + a.y + a.z + a.w;
}

OBEL_VECTOR_FUNC float obel_vector_reduce_mul(obel_vector3_t a) {
    return a.x * a.y * a.z;
}

OBEL_VECTOR_FUNC float obel_vector_reduce_mul(obel_vector4_t a) {
    return a.x * a.y * a.z * a.w;
}

OBEL_VECTOR_FUNC float obel_vector_reduce_min(obel_vector3_t a) {
    return MIN(MIN(a.x, a.y), a.z);
}

OBEL_VECTOR_FUNC float obel_vector_reduce_min(obel_vector4_t a) {
    return MIN(MIN(MIN(a.x, a.y), a.z), a.w);
}

OBEL_VECTOR_FUNC float obel_vector_reduce_max(obel_vector3_t a) {
    return MAX(MAX(a.x, a.y), a.z);
}

OBEL_VECTOR_FUNC float obel_vector_reduce_max(obel_vector4_t a) {
    return MAX(MAX(MAX(a.x, a.y), a.z), a.w);
}

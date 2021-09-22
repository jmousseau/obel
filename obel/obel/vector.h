//
//  vector.h
//  obel
//
//  Created by Jack Mousseau on 9/21/21.
//

#ifndef vector_h
#define vector_h

#include <obel/base.h>

#define OBEL_VECTOR_FUNC OBEL_INLINE OBEL_OVERLOADABLE OBEL_CONST

typedef union obel_vector3 {
    float elements[3];
    struct { float x, y, z; };
    struct { float r, g, b; };
} obel_vector3_t;

typedef union obel_vector4 {
    float elements[4];
    struct { union { struct { float x, y, z; }; obel_vector3_t xyz; }; float w; };
    struct { union { struct { float r, g, b; }; obel_vector3_t rgb; }; float a; };
} obel_vector4_t;

// MARK: - Geometry

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_cross(obel_vector3_t a, obel_vector3_t b);

OBEL_VECTOR_FUNC float obel_vector_distance(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC float obel_vector_distance(obel_vector4_t a, obel_vector4_t b);

OBEL_VECTOR_FUNC float obel_vector_dot(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC float obel_vector_dot(obel_vector4_t a, obel_vector4_t b);

OBEL_VECTOR_FUNC float obel_vector_length(obel_vector3_t a);
OBEL_VECTOR_FUNC float obel_vector_length(obel_vector4_t a);

// MARK: - Logic

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_min(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_min(obel_vector4_t a, obel_vector4_t b);

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_max(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_max(obel_vector4_t a, obel_vector4_t b);

// MARK: - Math

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_add(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC obel_vector3_t obel_vector_add(obel_vector3_t a, float b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_add(obel_vector4_t a, obel_vector4_t b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_add(obel_vector4_t a, float b);

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_sub(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC obel_vector3_t obel_vector_sub(obel_vector3_t a, float b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_sub(obel_vector4_t a, obel_vector4_t b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_sub(obel_vector4_t a, float b);

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_mul(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC obel_vector3_t obel_vector_mul(obel_vector3_t a, float b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_mul(obel_vector4_t a, obel_vector4_t b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_mul(obel_vector4_t a, float b);

OBEL_VECTOR_FUNC obel_vector3_t obel_vector_div(obel_vector3_t a, obel_vector3_t b);
OBEL_VECTOR_FUNC obel_vector3_t obel_vector_div(obel_vector3_t a, float b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_div(obel_vector4_t a, obel_vector4_t b);
OBEL_VECTOR_FUNC obel_vector4_t obel_vector_div(obel_vector4_t a, float b);

// MARK: - Reduction

OBEL_VECTOR_FUNC float obel_vector_reduce_add(obel_vector3_t a);
OBEL_VECTOR_FUNC float obel_vector_reduce_add(obel_vector4_t a);

OBEL_VECTOR_FUNC float obel_vector_reduce_mul(obel_vector3_t a);
OBEL_VECTOR_FUNC float obel_vector_reduce_mul(obel_vector4_t a);

OBEL_VECTOR_FUNC float obel_vector_reduce_min(obel_vector3_t a);
OBEL_VECTOR_FUNC float obel_vector_reduce_min(obel_vector4_t a);

OBEL_VECTOR_FUNC float obel_vector_reduce_max(obel_vector3_t a);
OBEL_VECTOR_FUNC float obel_vector_reduce_max(obel_vector4_t a);

#endif // vector_h

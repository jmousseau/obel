//
//  aabb.c
//  obel
//
//  Created by Jack Mousseau on 9/22/21.
//

#include <obel/aabb.h>

// MARK: - Metrics

OBEL_AABB_FUNC float obel_aabb_volume(obel_aabb_t a) {
    return obel_vector_reduce_mul(obel_vector_sub(a.max, a.min));
}

OBEL_AABB_FUNC float obel_aabb_iou(obel_aabb_t a, obel_aabb_t b) {
    float ab_volume = obel_aabb_volume(obel_aabb_intersection(a, b));
    return ab_volume / (obel_aabb_volume(a) + obel_aabb_volume(b) - ab_volume);
}

// MARK: - Intersection

OBEL_AABB_FUNC obel_aabb_t obel_aabb_intersection(obel_aabb_t a, obel_aabb_t b) {
    obel_vector3_t min = obel_vector_max(a.min, b.min);
    obel_vector3_t max = obel_vector_min(a.max, b.max);
    return (obel_aabb_t){ min, max };
}

OBEL_AABB_FUNC obel_aabb_t obel_aabb_union(obel_aabb_t a, obel_aabb_t b) {
    obel_vector3_t min = obel_vector_min(a.min, b.min);
    obel_vector3_t max = obel_vector_max(a.max, b.max);
    return (obel_aabb_t){ min, max };
}

OBEL_AABB_FUNC obel_aabb_t obel_aabb_union(obel_aabb_t a, obel_vector3_t b) {
    obel_vector3_t min = obel_vector_min(a.min, b);
    obel_vector3_t max = obel_vector_max(a.max, b);
    return (obel_aabb_t){ min, max };
}

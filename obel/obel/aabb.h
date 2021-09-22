//
//  aabb.h
//  obel
//
//  Created by Jack Mousseau on 9/22/21.
//

#ifndef aabb_h
#define aabb_h

#include <obel/base.h>
#include <obel/vector.h>

#define OBEL_AABB_FUNC OBEL_INLINE OBEL_OVERLOADABLE OBEL_CONST

typedef struct obel_aabb {
    obel_vector3_t min, max;
} obel_aabb_t;

// MARK: - Metrics

OBEL_AABB_FUNC float obel_aabb_volume(obel_aabb_t a);
OBEL_AABB_FUNC float obel_aabb_iou(obel_aabb_t a, obel_aabb_t b);

// MARK: - Intersection

OBEL_AABB_FUNC obel_aabb_t obel_aabb_intersection(obel_aabb_t a, obel_aabb_t b);
OBEL_AABB_FUNC obel_aabb_t obel_aabb_union(obel_aabb_t a, obel_aabb_t b);

#endif // aabb_h

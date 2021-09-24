//
//  cloud.h
//  obel
//
//  Created by Jack Mousseau on 9/23/21.
//

#ifndef cloud_h
#define cloud_h

#include <obel/aabb.h>
#include <obel/define.h>
#include <obel/histogram.h>
#include <obel/vector.h>

#define OBEL_CLOUND_FUNC OBEL_OVERLOADABLE

typedef struct obel_cloud {
    uint32_t capacity;
    uint32_t count;
    obel_vector3_t *points;
} obel_cloud_t;

typedef struct obel_cloud_stat {
    obel_vector3_t center_of_mass;
    obel_aabb_t aabb;
} obel_cloud_stat_t;

OBEL_CLOUND_FUNC obel_aabb_t obel_cloud_aabb(obel_cloud_t *cloud);
OBEL_CLOUND_FUNC obel_cloud_stat_t obel_cloud_stat(obel_cloud_t *cloud);

OBEL_CLOUND_FUNC void obel_cloud_l2_histogram_fit(obel_aabb_t aabb, obel_histogram_t *histogram);
OBEL_CLOUND_FUNC void obel_cloud_l2_histogram_bin(obel_cloud_t *cloud, uint32_t samples, obel_histogram_t *histogram);

#endif // cloud_h

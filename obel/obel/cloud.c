//
//  cloud.c
//  obel
//
//  Created by Jack Mousseau on 9/23/21.
//

#include <obel/cloud.h>

OBEL_CLOUND_FUNC obel_aabb_t obel_cloud_aabb(obel_cloud_t *cloud) {
    obel_aabb_t aabb = OBEL_AABB_EMPTY;
    for (uint32_t i = 0; i < cloud->count; i++) {
        aabb = obel_aabb_union(aabb, cloud->points[i]);
    }

    return aabb;
}

OBEL_CLOUND_FUNC obel_cloud_stat_t obel_cloud_stat(obel_cloud_t *cloud) {
    obel_cloud_stat_t stat = { .center_of_mass = OBEL_VECTOR3_ZERO, .aabb = OBEL_AABB_EMPTY };
    for (uint32_t i = 0; i < cloud->count; i++) {
        obel_vector3_t point = cloud->points[i];
        stat.center_of_mass = obel_vector_add(stat.center_of_mass, point);
        stat.aabb = obel_aabb_union(stat.aabb, point);
    }

    stat.center_of_mass = obel_vector_div(stat.center_of_mass, (float)cloud->count);
    return stat;
}

OBEL_CLOUND_FUNC void obel_cloud_l2_histogram_fit(obel_aabb_t aabb, obel_histogram_t *histogram) {
    OBEL_ASSERT(histogram->count > 0);
    histogram->offset = 0.f;
    histogram->stride = obel_vector_distance(aabb.min, aabb.max) / (float)histogram->count;
}

OBEL_CLOUND_FUNC void obel_cloud_l2_histogram_bin(obel_cloud_t *cloud, uint32_t samples, obel_histogram_t *histogram) {
    uint32_t max_samples = OBEL_MIN(samples, cloud->count * (cloud->count - 1) / 2);
    for (uint32_t i = 0; i < max_samples; i++) {
        obel_vector3_t point_a = cloud->points[arc4random_uniform(cloud->count)];
        obel_vector3_t point_b = cloud->points[arc4random_uniform(cloud->count)];
        obel_histogram_bin(histogram, obel_vector_distance(point_a, point_b));
    }
}

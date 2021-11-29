//
//  histogram.c
//  obel
//
//  Created by Jack Mousseau on 9/22/21.
//

#include <obel/histogram.h>

OBEL_HISTOGRAM_FUNC int32_t obel_histogram_bin(obel_histogram_t *histogram, float element) {
    int32_t bin = obel_histogram_bin(histogram->offset, histogram->stride, element);
    OBEL_ASSERT(bin >= 0 && bin < histogram->count);
    histogram->bins[bin]++;
    histogram->samples++;
    return bin;
}

OBEL_HISTOGRAM_FUNC int32_t obel_histogram_bin(float offset, float stride, float element) {
    return (int32_t)floorf((element - offset) / stride);
}

OBEL_HISTOGRAM_FUNC int32_t obel_histogram_distance(obel_histogram_t *a, obel_histogram_t *b) {
    OBEL_ASSERT(a->offset == b->offset && a->stride == b->stride && a->count == b->count);

    int32_t distance = 0;
    for (int32_t i = 0; i < a->count; i++) {
        distance += abs(a->bins[i] - b->bins[i]);
    }

    return distance;
}

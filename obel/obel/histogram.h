//
//  histogram.h
//  obel
//
//  Created by Jack Mousseau on 9/22/21.
//

#ifndef histogram_h
#define histogram_h

#include <obel/base.h>

#define OBEL_HISTOGRAM_FUNC OBEL_OVERLOADABLE OBEL_CONST

typedef struct obel_histogram {
    float offset;
    float stride;
    int32_t count;
    int32_t *bins;
} obel_histogram_t;

OBEL_HISTOGRAM_FUNC int32_t obel_histogram_bin(obel_histogram_t *histogram, float element);
OBEL_HISTOGRAM_FUNC int32_t obel_histogram_bin(float offset, float stride, float element);
OBEL_HISTOGRAM_FUNC int32_t obel_histogram_distance(obel_histogram_t *a, obel_histogram_t *b);

#endif // histogram_h

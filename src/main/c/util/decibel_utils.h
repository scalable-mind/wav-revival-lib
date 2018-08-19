#ifndef DECIBEL_UTILS_H
#define DECIBEL_UTILS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    BIT_8 = 1,
    BIT_16,
    BIT_24,
    BIT_32
} BitDepth;

typedef struct {

    bool _is_initialized;

    double (*threshold)(BitDepth d);

    double (*ratio)(int16_t v);

    int16_t (*to_val)(double v);

    double (*coefficient)(double v);

    bool (*greater)(int16_t src, double threshold);

} DecibelUtils;

DecibelUtils* decibel_utils();

#endif

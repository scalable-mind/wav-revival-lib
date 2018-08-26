#ifndef SIGNAL_FADING_UTILS_H
#define SIGNAL_FADING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include <domain/uint64_array.h>
#include <domain/wav_compressed_data.h>

typedef enum {
    FADE_OUT_FULL = -1,
    FADE_IN_FULL = -2,
    FADE_OUT_CROSS = -3,
    FADE_IN_CROSS = -4,
    FADE_START = -5,
    FADE_END = -6,
    FADE_NORMAL = -7
} SignalFadingState;

typedef struct {

    bool _is_initialized;

    void (*fade_in)(DoubleArray* dst, size_t start, size_t end, size_t sample_count, double ratio);

    void (*fade_out)(DoubleArray* dst, size_t start, size_t end, size_t sample_count, double ratio);

    void (*fade_expand)(DoubleArray* dst, size_t fade_in_len, size_t fade_out_len, uint64_t zero_count,
                     double ratio, SignalFadingState flag);

} SignalFadingUtils;

SignalFadingUtils* signal_fading_utils();

#endif

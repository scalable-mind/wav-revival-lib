#ifndef SIGNAL_FADING_UTILS_H
#define SIGNAL_FADING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include <util/decibel_utils.h>
#include <domain/uint64_array.h>

#include <macros/export.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    bool _is_initialized;

    void (*fade_in)(DoubleArray* dst, size_t start, size_t end, size_t sample_count, Decibel gain);

    void (*fade_out)(DoubleArray* dst, size_t start, size_t end, size_t sample_count, Decibel gain);

    void (*fade_expand_start)(DoubleArray* dst, size_t fade_in_len, size_t silence_len, Decibel gain);

    void (*fade_expand_normal)(DoubleArray* dst, size_t fade_in_len, size_t fade_out_len, size_t silence_len,
                               Decibel gain);

    void (*fade_expand_end)(DoubleArray* dst, size_t fade_out_len, size_t silence_len, Decibel gain);

} SignalFadingUtils;

MODULE_API SignalFadingUtils* signal_fading_utils();

#ifdef __cplusplus
}
#endif

#endif

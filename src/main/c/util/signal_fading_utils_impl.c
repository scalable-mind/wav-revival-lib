#include <stdlib.h>
#include <math.h>

#define MODULE_API_EXPORTS

#include <api/double_array_api.h>
#include <util/signal_fading_utils.h>
#include <util/decibel_utils.h>
#include <domain/double_array.h>

static inline size_t subtract(size_t a, size_t b) {
    return  (a < b ? b - a : a - b);
}

static void fade_in(DoubleArray* dst, size_t start, size_t end, size_t sample_count, Decibel gain) {
    double target = decibel_utils()->spl_to_ratio(-fabs(gain));
    for (uint64_t i = start; i < end; i++) {
        double_array_api()->push_back(dst, 1 + (target - 1) * (double) i / sample_count);
    }
}

static void fade_out(DoubleArray* dst, size_t start, size_t end, size_t sample_count, Decibel gain) {
    double target = decibel_utils()->spl_to_ratio(-fabs(gain));
    for (uint64_t i = start; i < end; i++) {
        double_array_api()->push_back(dst, target + (1 - target) * (double) i / sample_count);
    }
}

static void fade_expand_start(DoubleArray* dst, size_t fade_in_len, size_t silence_len, Decibel gain) {
    double_array_api()->push_some(dst, subtract(silence_len, fade_in_len), 1);
    fade_in(dst, 0, fade_in_len, fade_in_len, gain);
}

static void fade_expand_normal(DoubleArray* dst, size_t fade_in_len, size_t fade_out_len,
                               size_t silence_len, Decibel gain) {
    fade_out(dst, 0, fade_out_len, fade_out_len, gain);
    double_array_api()->push_some(dst, subtract(silence_len, (fade_in_len + fade_out_len)), 1);
    fade_in(dst, 0, fade_in_len, fade_in_len, gain);
}

static void fade_expand_end(DoubleArray* dst, size_t fade_out_len, size_t silence_len, Decibel gain) {
    if (fade_out_len > silence_len) {
        fade_out(dst, 0, silence_len, fade_out_len, gain);
    } else {
        fade_out(dst, 0, fade_out_len, fade_out_len, gain);
        double_array_api()->push_some(dst, subtract(silence_len, fade_out_len), 1);
    }
}

SignalFadingUtils* signal_fading_utils() {
    static SignalFadingUtils instance = {._is_initialized=false};

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.fade_in = fade_in;
        instance.fade_out = fade_out;
        instance.fade_expand_start = fade_expand_start;
        instance.fade_expand_normal = fade_expand_normal;
        instance.fade_expand_end = fade_expand_end;
    }

    return &instance;
}

#undef MODULE_API_EXPORTS

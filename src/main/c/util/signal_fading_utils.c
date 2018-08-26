#include <stdlib.h>
#include <math.h>

#include <api/double_array_api.h>
#include <util/signal_fading_utils.h>
#include <util/decibel_utils.h>
#include <domain/double_array.h>

static void fade_in(DoubleArray* dst, size_t start, size_t end, size_t sample_count, double ratio) {
    double target = decibel_utils()->coefficient(-fabs(ratio));
    for (uint64_t i = start; i < end; i++) {
        double_array_api().push_back(dst, 1 + (target - 1) * (double)i / sample_count);
    }
}

static void fade_out(DoubleArray* dst, size_t start, size_t end, size_t sample_count, double ratio) {
    double target = decibel_utils()->coefficient(-fabs(ratio));
    for (uint64_t i = start; i < end; i++) {
        double_array_api().push_back(dst, target + (1 - target) * (double)i / sample_count);
    }
}

static void fade_expand(DoubleArray* dst, size_t fade_in_len, size_t fade_out_len, uint64_t zero_count, double ratio,
                       SignalFadingState flag) {
    switch (flag) {
        case FADE_NORMAL:
                fade_out(dst, 0, fade_out_len, fade_out_len, ratio);
                double_array_api().push_some(dst, (zero_count - (fade_in_len + fade_out_len)), 1);
                fade_in(dst, 0, fade_in_len, fade_in_len, ratio);
            break;
        case FADE_START:
            double_array_api().push_some(dst, (zero_count - fade_in_len), 1);
            fade_in(dst, 0, fade_in_len, fade_in_len, ratio);
            break;
        case FADE_END:
            if (fade_out_len > zero_count) {
                fade_out(dst, 0, zero_count, fade_out_len, ratio);
            } else {
                fade_out(dst, 0, fade_out_len, fade_out_len, ratio);
                double_array_api().push_some(dst, (zero_count - fade_out_len), 1);
            }
            break;
        default:
            exit(1);
    }
}

SignalFadingUtils* signal_fading_utils() {
    static SignalFadingUtils instance = {._is_initialized=false};

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.fade_in = fade_in;
        instance.fade_out = fade_out;
        instance.fade_expand = fade_expand;
    }

    return &instance;
}
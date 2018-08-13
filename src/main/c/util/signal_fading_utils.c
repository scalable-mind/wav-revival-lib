#include <stdlib.h>

#include <api/double_array_api.h>
#include <util/signal_fading_utils.h>
#include <domain/double_array.h>
#include <assert.h>
#include "signal_fading_utils.h"

static bool crossfade(size_t* cross_index, size_t* fade_start_index,
                      size_t fade_in_len, size_t fade_out_len, uint64_t zero_count) {
    if (fade_in_len + fade_out_len <= zero_count) {
        return false;
    }
    *cross_index = (fade_out_len * fade_in_len + fade_out_len * (zero_count - fade_in_len))
                   / (fade_out_len + fade_in_len);
    *fade_start_index = *cross_index - (zero_count-fade_in_len);
    return true;
}

static void fade_in(DoubleArray* dst, size_t start, size_t end, size_t sample_count, double ratio) {
    for(uint64_t i = start; i < end; i++)
        double_array_api().push_back(dst, ((double)i/sample_count) * ratio);
}

static void fade_out(DoubleArray* dst, size_t start, size_t end, size_t sample_count, double ratio) {
    for(uint64_t i = start; i < end; i++)
        double_array_api().push_back(dst, (1 + -(double)i/sample_count) * ratio);
}

static void fade_expand(DoubleArray* dst, size_t cross_index, size_t fade_start_index,
                        size_t fade_in_len, size_t fade_out_len, uint64_t zero_count,
                        double ratio, SignalFadingState flag, bool has_cross) {
    switch(flag) {
        case FADE_NORMAL:
            if(has_cross) {
                fade_out(dst, 0, fade_out_len, fade_out_len, ratio);
                double_array_api().push_some(dst, (zero_count - (fade_in_len + fade_out_len)), 0);
                fade_in(dst, 0, fade_in_len, fade_in_len, ratio);
            } else {
                fade_out(dst, 0, cross_index, fade_out_len, ratio);
                fade_in(dst, fade_start_index, fade_in_len, fade_in_len, ratio);
            }
            break;
        case FADE_START:
            double_array_api().push_some(dst, (zero_count - fade_in_len), 0);
            fade_in(dst, 0, fade_in_len, fade_in_len, ratio);
            break;
        case FADE_END:
            fade_out(dst, 0, fade_out_len, fade_out_len, ratio);
            double_array_api().push_some(dst, (zero_count - fade_out_len), 0);
        default:
            exit(1);
    }
}

SignalFadingUtils* signal_fading_utils() {
    static SignalFadingUtils instance = {._is_initialized=false};

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.crossfade = crossfade;
        instance.fade_in = fade_in;
        instance.fade_out = fade_out;
        instance.fade_expand = fade_expand;
    }

    return &instance;
}
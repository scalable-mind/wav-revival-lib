#include <stdbool.h>
#include <stdlib.h>

#include <api/wav_file_api.h>
#include <api/uint64_array_api.h>
#include <util/decibel_utils.h>
#include <util/wav_compressing_utils.h>
#include <domain/compressed_data.h>
#include <domain/uint64_array.h>

/**
 * @param value
 * @param threshold
 * @return Silence-filtered Value (STFV_ABOVE or STFV_BELOW)
 */
static inline Stfv filter_signal(Amp value, Amp threshold) {
    return (abs(value) > threshold ? STFV_ABOVE : STFV_BELOW);
}

/**
 * @param smooth_region_end - explained below
 * @param silence - the length (number of WAV samples) of silence allowed.
 * @param filtered_value - indicates whether the WAV sample has exceeded the silence level threshold or not.
 * @param samples_count - the length of consecutive silence or non-silence WAV samples.
 * @param dst - a storage of compressed values.
 *
 * Smoothing is the part of compression algorithm. Here we compare the length of silence region (a) with allowed length
 * of silence (b). Let's name any of non-silence regions as (ci).
 * Therefore, there are 2 types of smooth region:
 *
 * 1. Non-smooth - if (a) > (b), the region consists of exactly one element - (a). In this state, (a) is just pushed
 *    back to the resulting array, and the `smooth_region_end` is set to false;
 *
 * 2. Smooth - if (a) <= (b), the region consists of 3 elements with the following pattern: [(c1), (a), (c2)]. In
 *    this state, resulting array contains the sum of these: (c1) + (a) + (c2).
 *
 * The algorithm defines the type of a smooth region only when stopped calculating the (a). At this moment,
 * the last element of resulting array is (c1); (a) is contained in `samples_count`, and (c2) is not yet calculated.
 * The compressing algorithm calls this function, where we choose the type of smooth region.
 *
 * Let's break down the situation where the 2-nd type smooth region appears:
 * <pre>
 * | # | dst[size - 1] | filtered_value  | smooth_region_end | samples_count |<br>
 * |---|---------------|-----------------|-------------------|---------------|<br>
 * | 1 | (c1)          | STFV_BELOW      | false             | (a)           |<br>
 * | 2 | (a)           | STFV_ABOVE      | true              | (c2)          |<br>
 * | 3 | (c2)          | STFV_BELOW      | false             | ...           |<br>
 * </pre>
 *
 * At the step 1, we hit condition `filtered_value == STF_BELOW && samples_count <= silence` (with respect to the
 * size check of `dst`), add up `dst[size - 1]` and (a) and invert `smooth_region_end` to true.
 * At the step 2, we hit condition `filtered_value == STF_ABOVE && *smooth_region_end`, add up `dst[size - 1]` and (c2)
 * and, finally, invert `smooth_region_end` back to false.
 * The step 3 and further may continue the same pattern as the steps (1 - 2) that represent the 2-nd type of smooth
 * region, or may lead to the 1-st type, which is simpler to understand.
 */
static void process_smooth_region(bool* smooth_region_end, size_t silence, Stfv filtered_value, size_t samples_count,
                                  CompressedData* dst) {
    if (
            (filtered_value == STFV_ABOVE && *smooth_region_end) ||
            (filtered_value == STFV_BELOW && samples_count <= silence && dst->compressed_data->_size > 0)
    ) {
        *(uint64_array_api()->end(dst->compressed_data) - 1) += samples_count;
        *smooth_region_end = !(*smooth_region_end);
    } else {
        uint64_array_api()->push_back(dst->compressed_data, samples_count);
    }
}

static void compress_smooth_rsp_chunk(Amp* data, size_t data_size, bool start_chunk, bool* smooth_region_end,
                                      Amp threshold, size_t silence, Stfv* filtered_value,
                                      size_t* samples_count, CompressedData* dst) {
    Stfv next_filtered_value;

    size_t i = 0;

    if (data_size > 0 && start_chunk) {
        dst->start_value = filter_signal(data[0], threshold);
        (*samples_count)++;
        *filtered_value = dst->start_value;
        i++;
    }

    for (; i < data_size; i++) {
        next_filtered_value = filter_signal(data[i], threshold);

        if (*filtered_value != next_filtered_value) {
            process_smooth_region(smooth_region_end, silence, *filtered_value, *samples_count, dst);
            *samples_count = 0;
        }

        (*samples_count)++;
        *filtered_value = next_filtered_value;
    }
}

WavCompressingUtils* wav_compressing_utils() {
    static WavCompressingUtils instance = { ._is_initialized=false };

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.compress_smooth_rsp_chunk = compress_smooth_rsp_chunk;
    }
    return &instance;
}

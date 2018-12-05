#include <stdbool.h>
#include <stdlib.h>

#include <api/wav_file_api.h>
#include <api/uint64_array_api.h>
#include <util/decibel_utils.h>
#include <util/wav_compressing_utils.h>
#include <domain/compressed_data.h>
#include <domain/uint64_array.h>

static void compress_int16_array(int16_t data[], size_t data_size, bool start_chunk,
                                 int16_t threshold, int8_t* prev_val, size_t* samples_count, CompressedData* dst) {
    int8_t norm_val;

    for (size_t i = 0; i < data_size; i++) {
        norm_val = (int8_t) (abs(data[i]) > threshold ? 1 : 0);

        if(start_chunk) {
            dst->start_value = norm_val;
            start_chunk = false;
        } else if (*prev_val != norm_val) {
            uint64_array_api().push_back(dst->compressed_data, *samples_count);
            *samples_count = 0;
        }

        (*samples_count)++;
        *prev_val = norm_val;
    }

    uint64_array_api().shrink_to_fit(dst->compressed_data);
}

static void compress_smooth_int16_array(int16_t data[], size_t data_size, bool start_chunk, bool* fill_ones,
                                        int16_t threshold, size_t silence, int8_t* prev_val,
                                        size_t* samples_count, CompressedData* dst) {
    int8_t norm_val;

    for (size_t i = 0; i < data_size; i++) {
        norm_val = (int8_t) (abs(data[i]) > threshold ? 1 : 0);

        if(start_chunk) {
            dst->start_value = norm_val;
            start_chunk = false;
        } else if (*prev_val != norm_val) {
            if ((*prev_val == 1 && *fill_ones) ||
                (*prev_val == 0 && *samples_count <= silence
                 && dst->compressed_data->size > 0)) {
                *(uint64_array_api().end(dst->compressed_data) - 1) += *samples_count;
                *fill_ones = !(*fill_ones);
            }
            else {
                uint64_array_api().push_back(dst->compressed_data, *samples_count);
            }
//            if (*prev_val == 1) {                                                          // !A
//                if(*fill_ones) {                                                           // B
//                    *(uint64_array_api().end(dst->compressed_data) - 1) += *samples_count; // a
//                    *fill_ones = !(*fill_ones);                                            // b
//                }
//                else {
//                    uint64_array_api().push_back(dst->compressed_data, *samples_count);
//                }
//            } else if (dst->compressed_data->size > 0 && *prev_val == 0) {                 // A && D
//                if(*samples_count > silence) {                                             // C
//                    uint64_array_api().push_back(dst->compressed_data, *samples_count);    // c
//                }
//                else {
//                    *(uint64_array_api().end(dst->compressed_data) - 1) += *samples_count; // a
//                    *fill_ones = !(*fill_ones);                                            // b
//                }
//            }

            *samples_count = 0;
        }

        (*samples_count)++;
        *prev_val = norm_val;
    }

    uint64_array_api().shrink_to_fit(dst->compressed_data);
}

WavCompressingUtils* wav_compressing_utils() {
    static WavCompressingUtils instance = { ._is_initialized=false };

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.compress_int16_array = compress_int16_array;
        instance.compress_smooth_int16_array = compress_smooth_int16_array;
    }
    return &instance;
}

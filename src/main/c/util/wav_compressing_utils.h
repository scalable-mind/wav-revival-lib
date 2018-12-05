#ifndef WAV_COMPRESSING_UTILS_H
#define WAV_COMPRESSING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include <domain/compressed_data.h>
#include <domain/wav_file.h>

typedef struct {

    bool _is_initialized;

    void (*compress_int16_array)(int16_t data[], size_t data_size, bool start_chunk,
                                     int16_t threshold, int8_t* prev_val, size_t* samples_count, CompressedData* dst);

    void (*compress_smooth_int16_array)(int16_t data[], size_t data_size, bool start_chunk, bool* fill_ones,
                                     int16_t threshold, size_t silence, int8_t* prev_val,
                                     size_t* samples_count, CompressedData* dst);

} WavCompressingUtils;

WavCompressingUtils* wav_compressing_utils();

#endif

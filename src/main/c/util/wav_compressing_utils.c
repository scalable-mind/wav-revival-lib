#include <stdbool.h>
#include <stdlib.h>

#include <api/wav_file_api.h>
#include <api/uint64_array_api.h>
#include <util/wav_compressing_utils.h>
#include <domain/wav_compressed_data.h>

static void compress(WavFile* wav_file, WavCompressedData* data, uint16_t threshold) {
    bool start = true;
    uint64_t previous_value;
    uint64_t samples_count = 0;

    //NOTE: We assume the source WAV file is INT16BIT MONO
    int16_t* buffer = calloc(wav_file->buffer_size, sizeof(int16_t));
    size_t buffer_size;
    while ((buffer_size = wav_file_api().read_next_chunk(wav_file, buffer)) != 0) {
        for (size_t i = 0; i < buffer_size; i++) {
            uint64_t normalized_value = abs(buffer[i]) > threshold ? 1 : 0;

            if (start) {
                data->start_value = (int) normalized_value;
                start = false;
            } else if (previous_value != normalized_value) {
                uint64_array_api().push_back(data->compressed_data, samples_count);
                samples_count = 0;
            }

            samples_count++;
            previous_value = normalized_value;
        }
    }
    uint64_array_api().push_back(data->compressed_data, samples_count);
    uint64_array_api().shrink_to_fit(data->compressed_data);
    free(buffer);
}

WavCompressingUtils* wav_compressing_utils() {
    static WavCompressingUtils instance = { ._is_initialized=false };

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.compress = compress;
    }
    return &instance;
}

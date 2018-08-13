#ifndef WAV_COMPRESSING_UTILS_H
#define WAV_COMPRESSING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include <domain/wav_compressed_data.h>
#include <domain/wav_file.h>

typedef enum {
    ZERO_STATE = 0,
    ONE_STATE = 1
} SampleState;

typedef struct {

    bool _is_initialized;

    void (*compress)(WavFile* input, WavCompressedData* data, uint16_t threshold);

    void (*smooth)(WavCompressedData* data, size_t silence);

} WavCompressingUtils;

WavCompressingUtils* wav_compressing_utils();

#endif

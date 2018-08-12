#ifndef WAV_COMPRESSING_UTILS_H
#define WAV_COMPRESSING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include <domain/wav_compressed_data.h>
#include <domain/wav_file.h>

typedef struct {

    bool _is_initialized;

    void (*compress)(WavFile* input, WavCompressedData* data, uint16_t threshold);

} WavCompressingUtils;

WavCompressingUtils* wav_compressing_utils();

#endif

#ifndef WAV_HEADER_H
#define WAV_HEADER_H

#include <stdint.h>

// NOTE: we use standard PCM RIFF WAV header with 44 bytes size.
typedef struct {

    char chunk_id[4];

    uint32_t chunk_size;

    char format[4];

    char subchunk_id[4];

    uint32_t subchunk_size;

    uint16_t audio_format;

    uint16_t num_channels;

    uint32_t sample_rate;

    uint32_t byte_rate;

    uint16_t block_align;

    uint16_t bits_per_sample;

    char data_id[4];

    uint32_t data_size;

} WavHeader;

#endif

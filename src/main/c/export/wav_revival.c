#include <stdlib.h>

#define MODULE_API_EXPORTS

#include "../api.h"
#include "../util.h"
#include "../domain.h"
#include "wav_revival.h"
#include "../macros/export.h"


#define SIZE_KBYTE (1024)
#define SIZE_MBYTE (1024*SIZE_KBYTE)
#define BUFFER_SIZE (5 * SIZE_MBYTE)

void voice_over(
        const char* orig_path,
        const char* voice_path,
        const char* output_path,
        size_t attack,
        size_t release,
        size_t silence,
        double threshold,
        double gain) {

    WavFile* orig_file = wav_file_api()->init(orig_path, "r", BUFFER_SIZE);
    WavFile* voice_file = wav_file_api()->init(voice_path, "r", BUFFER_SIZE);
    WavFile* output_file = wav_file_api()->init(output_path, "w", BUFFER_SIZE);

    if (orig_file->header.data_size > voice_file->header.data_size) {
        wav_file_api()->write_header(output_file, &(orig_file->header));
    } else {
        wav_file_api()->write_header(output_file, &(voice_file->header));
    }

    CompressedData cdata = {.compressed_data=uint64_array_api()->init(0)};

    {
        Amp* buffer = (Amp*) malloc(BUFFER_SIZE * sizeof(Amp));
        size_t chunk_size = 0;
        bool smooth_region_end = false;
        Stfv filtered_value = STFV_INIT;
        size_t last = 0;

        bool start_chunk = true;

        while ((chunk_size = wav_file_api()->read_next_chunk(voice_file, buffer)) != 0) {
            wav_compressing_utils()->compress_smooth_amp_chunk(
                    buffer, chunk_size, start_chunk, &smooth_region_end,
                    decibel_utils()->spl_to_amp(threshold), silence,
                    &filtered_value, &last, &cdata);
            if (start_chunk) {
                start_chunk = false;
            }
        }

        if (last > 0) {
            if (smooth_region_end) {
                *(uint64_array_api()->end(cdata.compressed_data) - 1) += last;
            } else {
                uint64_array_api()->push_back(cdata.compressed_data, last);
            }
        }

        free(buffer);
    }

    DoubleArray* coefficients = double_array_api()->init(BUFFER_SIZE);

    size_t current_value = (size_t) cdata.start_value;
    size_t start = 0;

    size_t fade_in_len = (size_t) (attack * voice_file->header.sample_rate);
    size_t fade_out_len = (size_t) (release * voice_file->header.sample_rate);

    size_t end = cdata.compressed_data->_size - 1;

    for (size_t i = 0; i <= end; i++) {
        if (i == 0) {
            if (current_value == 0) {
                signal_fading_utils()->fade_expand_start(
                        coefficients, fade_in_len,
                        cdata.compressed_data->_data[i], gain);
            } else {
                signal_fading_utils()->fade_expand_end(
                        coefficients, fade_out_len,
                        cdata.compressed_data->_data[i], gain);
            }
        } else {
            if (current_value == 0) {
                signal_fading_utils()->fade_expand_normal(
                        coefficients, fade_in_len, fade_out_len,
                        cdata.compressed_data->_data[i], gain);
            } else {
                double_array_api()->push_some(coefficients, cdata.compressed_data->_data[i], current_value);
            }

            if (i == end && current_value == 0) {
                signal_fading_utils()->fade_expand_end(
                        coefficients, fade_out_len,
                        cdata.compressed_data->_data[i], gain);
            }
        }
        current_value = (current_value == 1 ? 0 : 1);
    }

    uint64_array_api()->del(cdata.compressed_data);

    double_array_api()->shrink_to_fit(coefficients);
    wav_file_api()->refresh(voice_file);
    wav_file_api()->refresh(orig_file);

    DoubleIterator iterator = double_array_api()->begin(coefficients);
    int16_t* voice_buffer = calloc(voice_file->buffer_size,
                                   voice_file->header.block_align / voice_file->header.num_channels);
    int16_t* orig_buffer = calloc(orig_file->buffer_size,
                                  orig_file->header.block_align / orig_file->header.num_channels);
    int16_t* out_buffer = calloc(output_file->buffer_size,
                                 output_file->header.block_align / output_file->header.num_channels);

    size_t voice_buffer_size;
    size_t back_buffer_size;

    while ((voice_buffer_size = wav_file_api()->read_next_chunk(voice_file, voice_buffer)) != 0
           && (back_buffer_size = wav_file_api()->read_next_chunk(orig_file, orig_buffer)) != 0) {
        end = back_buffer_size / 2;
        if (back_buffer_size != output_file->buffer_size) {
            output_file->buffer_size = back_buffer_size;
        }

        for (size_t i = 0; i < end; i++) {
            if (iterator == double_array_api()->end(coefficients)) {
                break;
            }
            out_buffer[i * 2] = (int16_t) ((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                           orig_buffer[i * 2] * *iterator);
            out_buffer[i * 2 + 1] = (int16_t) ((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                               orig_buffer[i * 2 + 1] * *iterator);
            iterator++;
        }
    }

    free(out_buffer);
    free(orig_buffer);
    free(voice_buffer);

    double_array_api()->del(coefficients);

    wav_file_api()->del(output_file);
    wav_file_api()->del(voice_file);
    wav_file_api()->del(orig_file);
}

#undef MODULE_API_EXPORTS

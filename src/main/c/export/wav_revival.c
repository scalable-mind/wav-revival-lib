#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MODULE_API_EXPORTS

#include "../api.h"
#include "../util.h"
#include "../domain.h"
#include "wav_revival.h"
#include "../macros/export.h"

#define SIZE_KBYTE (1024)
#define SIZE_MBYTE (1024*SIZE_KBYTE)
#define BUFFER_SIZE (SIZE_MBYTE)

void voice_over(
        const char* orig_path,
        const char* voice_path,
        const char* output_path,
        double attack,
        double release,
        double silence,
        double threshold,
        double gain) {

    if (silence < attack + release) {
        silence = attack + release;
    }

    WavFile* orig_file = wav_file_api()->init(orig_path, "rb", BUFFER_SIZE);
    WavFile* voice_file = wav_file_api()->init(voice_path, "rb", BUFFER_SIZE);
    WavFile* output_file = wav_file_api()->init(output_path, "wb", BUFFER_SIZE);

    wav_file_api()->write_header(output_file, &(orig_file->header));

    size_t silence_samples = silence * voice_file->header.sample_rate;
    CompressedData cdata = {.compressed_data=uint64_array_api()->init(0)};

    {
        Amp* buffer = calloc(voice_file->buffer_size, sizeof(Amp));
        size_t chunk_size = 0;
        bool smooth_region_end = false;
        Stfv filtered_value = STFV_INIT;
        size_t last = 0;

        bool start_chunk = true;

        while ((chunk_size = wav_file_api()->read_next_chunk(voice_file, buffer)) != 0) {
            wav_compressing_utils()->compress_smooth_amp_chunk(
                    buffer, chunk_size, start_chunk, &smooth_region_end,
                    decibel_utils()->spl_to_amp(threshold), silence_samples,
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

    size_t total_samples = 0;

    for (size_t i = 0; i < cdata.compressed_data->_size; ++i) {
        total_samples += cdata.compressed_data->_data[i];
    }

    printf("Total samples: %llu, Total seconds: %.3lf\n",
           total_samples,
           total_samples / (double) voice_file->header.sample_rate);

    DoubleArray* coefficients = double_array_api()->init(total_samples);

    total_samples = 0;

    size_t current_value = (size_t) cdata.start_value;
    size_t start = 0;

    size_t fade_in_len = (size_t) (attack * voice_file->header.sample_rate);
    size_t fade_out_len = (size_t) (release * voice_file->header.sample_rate);

    size_t end = cdata.compressed_data->_size - 1;

    if (current_value == 0) {
        signal_fading_utils()->fade_expand_start(
                coefficients, fade_in_len,
                cdata.compressed_data->_data[0], gain);
        total_samples += cdata.compressed_data->_data[0];
        current_value = 1;
        start = 1;
    }

    double target = decibel_utils()->spl_to_ratio(-fabs(gain));

    for (size_t i = start; i <= end; i++) {
        if (current_value == 1) {
            double_array_api()->push_some(coefficients, cdata.compressed_data->_data[i], target);
        } else if (i == end) {
            signal_fading_utils()->fade_expand_end(
                    coefficients, fade_out_len,
                    cdata.compressed_data->_data[i], gain);

        } else {
            signal_fading_utils()->fade_expand_normal(
                    coefficients, fade_in_len, fade_out_len,
                    cdata.compressed_data->_data[i], gain);
        }

        total_samples += cdata.compressed_data->_data[i];
        current_value = (current_value == 1 ? 0 : 1);
    }

    uint64_array_api()->del(cdata.compressed_data);
    double_array_api()->shrink_to_fit(coefficients);
    wav_file_api()->refresh(voice_file);
    wav_file_api()->refresh(orig_file);

    DoubleIterator iterator = double_array_api()->begin(coefficients);
    Amp* voice_buffer = calloc(voice_file->buffer_size, sizeof(Amp));
    Amp* orig_buffer = calloc(orig_file->buffer_size, sizeof(Amp));
    Amp* out_buffer = calloc(output_file->buffer_size, sizeof(Amp));

    size_t voice_buffer_size = 0;
    size_t back_buffer_size = 0;

    size_t chunks_read = 0;

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
            out_buffer[i * 2] = (int16_t)((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                          orig_buffer[i * 2] * *iterator);
            out_buffer[i * 2 + 1] = (int16_t)((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                              orig_buffer[i * 2 + 1] * *iterator);
            iterator++;
        }
        wav_file_api()->write_next_chunk(output_file, out_buffer, back_buffer_size);
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

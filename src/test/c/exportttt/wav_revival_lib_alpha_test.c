#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include <domain/double_array.h>
#include <domain/wav_compressed_data.h>
#include <domain/wav_file.h>
#include <api/double_array_api.h>
#include <api/uint64_array_api.h>
#include <api/wav_file_api.h>
#include <util/signal_fading_utils.h>
#include <util/wav_compressing_utils.h>
#include <util/decibel_utils.h>
#include <domain/wav_header.h>
#include <domain/uint64_array.h>


void OverVoice(const char* voice, const char* back, const char* out,
               double attack, double release, double silence, double threshold, double ratio) {
    if (silence < attack + release) {
        silence =  attack + release;
    }
    WavFile* fvoice = wav_file_api().init(voice, READ, MB_1);
    WavFile* fback = wav_file_api().init(back, READ, MB_1);
    WavFile* fout = wav_file_api().init(out, WRITE, fback->buffer_size);

    if (fvoice->header.data_size < fback->header.data_size / 2) {
        fclose(fvoice->file);
        if (fopen_s(&(fvoice->file), voice, "r+b") == 0) {
            int16_t zero = 1;
            size_t extent = fback->header.data_size / fback->header.block_align -
                            fvoice->header.data_size / fvoice->header.block_align;
            fseek(fvoice->file, 0, SEEK_END);
            for (size_t i = 0; i < extent; i++) {
                fwrite(&zero, sizeof(int16_t), 1, fvoice->file);
            }
            fvoice->header.data_size += extent * fvoice->header.block_align;
            fseek(fvoice->file, 0, SEEK_SET);
            wav_file_api().write_header(fvoice, &(fvoice->header));
            wav_file_api().del(fvoice);
            fvoice = wav_file_api().init(voice, READ, MB_1);
        }
    } else if (fvoice->header.data_size > fback->header.data_size / 2) {
        fclose(fback->file);
        if (fopen_s(&(fback->file), voice, "r+b") == 0) {
            int16_t zero = 0;
            size_t extent = fvoice->header.data_size / fvoice->header.block_align -
                            fback->header.data_size / fback->header.block_align;
            fseek(fvoice->file, 0, SEEK_END);
            for (size_t i = 0; i < extent * 2; i++) {
                fwrite(&zero, sizeof(int16_t), 1, fback->file);
            }
            fback->header.data_size += extent * fback->header.block_align;
            fseek(fback->file, 0, SEEK_SET);
            wav_file_api().write_header(fback, &(fback->header));
            wav_file_api().del(fback);
            fback = wav_file_api().init(back, READ, MB_1);
        }
    }

    printf("Voice & Original track are aligned.\n");

    fout->header = fback->header;

    wav_file_api().write_header(fout, &(fback->header));

    WavCompressedData data = {.compressed_data=uint64_array_api().init(0)};

    wav_compressing_utils()->compress(fvoice, &data, (uint16_t) abs(decibel_utils()->to_val(threshold)));
    printf("wav_compressing_utils()->compress(...) works correctly.\n");
    wav_compressing_utils()->smooth(&data, (size_t) (silence * fvoice->header.sample_rate));
    printf("wav_compressing_utils()->smooth(...) works correctly.\n");

    wav_file_api().refresh(fvoice);

    int16_t* voice_buffer = calloc(fvoice->buffer_size, fvoice->header.block_align / fvoice->header.num_channels);
    int16_t* back_buffer = calloc(fback->buffer_size, fback->header.block_align / fback->header.num_channels);
    int16_t* out_buffer = calloc(fback->buffer_size, fback->header.block_align / fback->header.num_channels);

    size_t voice_buffer_size;
    size_t back_buffer_size;

    DoubleArray* coefficients = double_array_api().init(fvoice->header.data_size / fvoice->header.block_align);

    size_t current_value = (size_t) data.start_value;
    size_t start = 0;

    size_t fade_in_len = (size_t) (attack * fvoice->header.sample_rate);
    size_t fade_out_len = (size_t) (release * fvoice->header.sample_rate);

    SignalFadingState flag;

    if (current_value == 0) {
        flag = FADE_START;
        signal_fading_utils()->fade_expand(coefficients, fade_in_len, fade_out_len,
                                           data.compressed_data->data[0], ratio, flag);
        double_array_api().push_some(coefficients, data.compressed_data->data[1],
                                     decibel_utils()->coefficient(-fabs(ratio)));
        start = 2;
    }

    flag = FADE_NORMAL;

    size_t end = data.compressed_data->size - 1;

    for (size_t i = start; i <= end; i++) {
        if (i == end && current_value == 0) {
            flag = FADE_END;
        }
        if (current_value == 0) {
            signal_fading_utils()->fade_expand(coefficients, fade_in_len, fade_out_len,
                                               data.compressed_data->data[i], ratio, flag);
        } else {
            double_array_api().push_some(coefficients, data.compressed_data->data[i],
                                         decibel_utils()->coefficient(-fabs(ratio)));
        }

        current_value = (current_value == 1 ? 0 : 1);
    }
    double_array_api().shrink_to_fit(coefficients);
    DoubleIterator iterator = double_array_api().begin(coefficients);
    size_t chunk_count = 0;
    wav_file_api().refresh(fvoice);
    wav_file_api().refresh(fback);
    while ((voice_buffer_size = wav_file_api().read_next_chunk(fvoice, voice_buffer)) != 0
           && (back_buffer_size = wav_file_api().read_next_chunk(fback, back_buffer)) != 0) {
        end = back_buffer_size / 2;
        if (back_buffer_size != fout->buffer_size) {
            fout->buffer_size = back_buffer_size;
        }

        for (size_t i = 0; i < end; i++) {
            if (iterator == double_array_api().end(coefficients)) {
                break;
            }
            out_buffer[i * 2] = (int16_t) ((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                           back_buffer[i * 2] * *iterator);
            out_buffer[i * 2 + 1] = (int16_t) (
                    (i < voice_buffer_size ? voice_buffer[i] : 0) +
                                        back_buffer[i * 2 + 1] * *iterator);
            iterator++;
        }
        printf("chunk: %llu, written: %llu, actual: %llu;\n", ++chunk_count,
               wav_file_api().write_next_chunk(fout, out_buffer), back_buffer_size);
    }

    free(out_buffer);
    free(back_buffer);
    free(voice_buffer);
    double_array_api().del(coefficients);
    uint64_array_api().del(data.compressed_data);
    wav_file_api().del(fout);
    wav_file_api().del(fback);
    wav_file_api().del(fvoice);
}

void OverVoice__test() {
    OverVoice("D:/MCKunda/Documents/CODING/Scalable-mind/wav-revival-lib/src/test/resources/test-voice.wav",
              "D:/MCKunda/Documents/CODING/Scalable-mind/wav-revival-lib/src/test/resources/test-original.wav",
              "D:/MCKunda/Documents/CODING/Scalable-mind/wav-revival-lib/src/test/resources/test-result.wav",
              0.2, 1.3, 0.4, -30, 6);
}

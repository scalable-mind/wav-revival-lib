#include <assert.h>
#include <stdlib.h>

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
    WavFile* fvoice = wav_file_api().init(voice, READ, MB_1);
    WavFile* fback = wav_file_api().init(back, READ, MB_1);
    WavFile* fout = wav_file_api().init(out, WRITE, fback->buffer_size);
    wav_file_api().write_header(fout, &(fback->header));

    WavCompressedData data = {.compressed_data=uint64_array_api().init(0)};

    wav_compressing_utils()->compress(fvoice, &data, (uint16_t)abs(decibel_utils()->to_val(threshold)));
    assert(data.compressed_data->size == 2879319);
    printf("wav_compressing_utils()->compress(...) works correctly.\n");
    wav_compressing_utils()->smooth(&data, (size_t)(silence * fvoice->header.sample_rate));
    assert(data.compressed_data->size == 637);
    printf("wav_compressing_utils()->smooth(...) works correctly.\n");
//    FILE* file;
//    if (fopen_s(&file, "E:/data/tmp/wav-revival/inspect.txt", "w") == 0) {
//        for (size_t i = 0; i < data.compressed_data->size; i++) {
//            fprintf(file, "%llu\n", data.compressed_data->data[i]);
//        }
//        fclose(file);
//        exit(0);
//    }

    wav_file_api().refresh(fvoice);

    int16_t* voice_buffer = calloc(fvoice->buffer_size, fvoice->header.block_align / fvoice->header.num_channels);
    int16_t* back_buffer = calloc(fback->buffer_size, fback->header.block_align / fback->header.num_channels);
    int16_t* out_buffer = calloc(fback->buffer_size, fback->header.block_align / fback->header.num_channels);

    size_t voice_buffer_size;
    size_t back_buffer_size;

    DoubleArray* coefficients = double_array_api().init(fvoice->header.data_size / fvoice->header.block_align);

    size_t cross_index;
    size_t fade_start_index;

    size_t current_value = (size_t)data.start_value;
    size_t start = 0;

    size_t fade_in_len = (size_t)(attack * fvoice->header.sample_rate);
    size_t fade_out_len = (size_t)(release * fvoice->header.sample_rate);

    bool has_cross;

    SignalFadingState flag;

    if(current_value == 0) {
        flag = FADE_START;
        has_cross = signal_fading_utils()->crossfade(&cross_index, &fade_start_index, fade_in_len, fade_out_len,
                                                     data.compressed_data->data[0]);

        signal_fading_utils()->fade_expand(coefficients, cross_index, fade_start_index,
                                           (size_t)(attack * fvoice->header.sample_rate),
                                           (size_t)(release * fvoice->header.sample_rate),
                                           data.compressed_data->data[0], ratio, flag, has_cross);
        double_array_api().push_some(coefficients, data.compressed_data->data[1], 1);
        start = 2;
    }

    flag = FADE_NORMAL;

    size_t end = data.compressed_data->size - 1;

    for(size_t i = start; i <= end; i++) {
        if(i==end && current_value == 0) {
            flag = FADE_END;
        }
        if(current_value == 0) {
            has_cross = signal_fading_utils()->crossfade(&cross_index, &fade_start_index, fade_in_len, fade_out_len,
                                                         data.compressed_data->data[i]);
            signal_fading_utils()->fade_expand(coefficients, cross_index, fade_start_index,
                                               (size_t)(attack * fvoice->header.sample_rate),
                                               (size_t)(release * fvoice->header.sample_rate),
                                               data.compressed_data->data[i], ratio, flag, has_cross);
        } else {
            double_array_api().push_some(coefficients, data.compressed_data->data[i], current_value);
        }

        current_value = (current_value == 1 ? 0: 1);
    }
    double_array_api().shrink_to_fit(coefficients);
    DoubleIterator iterator = double_array_api().begin(coefficients);

    while ((voice_buffer_size = wav_file_api().read_next_chunk(fvoice, voice_buffer)) != 0
           && (back_buffer_size = wav_file_api().read_next_chunk(fback, back_buffer)) != 0) {
        end = back_buffer_size / 2;

        for (size_t i = 0; i < end; i++) {
            if(iterator == double_array_api().end(coefficients)) break;
            out_buffer[i*2] = (int16_t)(((i < voice_buffer_size) ? (voice_buffer[i] / 2.) : 0) + back_buffer[i*2] * decibel_utils()->to_val(*iterator));
            out_buffer[i*2+1] = (int16_t)(((i < voice_buffer_size) ? (voice_buffer[i] / 2.) : 0) + back_buffer[i*2+1] * decibel_utils()->to_val(*iterator));
            iterator++;
        }
        wav_file_api().write_next_chunk(fout, out_buffer);
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
    OverVoice("E:/data/tmp/wav-revival/test-voice.wav",
              "E:/data/tmp/wav-revival/test-original.wav",
              "E:/data/tmp/wav-revival/test-result.wav",
              0.2, 1.3, 0.4, -30, 15);
}

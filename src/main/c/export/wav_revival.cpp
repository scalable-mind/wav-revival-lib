#include <cstdlib>
#include <cmath>
#include <memory>

#define MODULE_API_EXPORTS

#include "../util.h"
#include "../domain.h"
#include "wav_revival.h"

static constexpr size_t SIZE_KBYTE = 1024;
static constexpr size_t SIZE_MBYTE  = 1024 * SIZE_KBYTE;
static constexpr size_t BUFFER_SIZE = SIZE_MBYTE;

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

    WavFile orig_file(orig_path, "rb", BUFFER_SIZE);
    WavFile voice_file(voice_path, "rb", BUFFER_SIZE);
    WavFile output_file(output_path, "wb", BUFFER_SIZE);

    output_file.write_header(&(orig_file.header));

    size_t silence_samples = static_cast<size_t>(silence * voice_file.header.sample_rate);
    CompressedData cdata{};

    {
        std::shared_ptr<Amp[]> buffer(new Amp[voice_file.buffer_size]);
        size_t chunk_size;
        bool smooth_region_end = false;
        Stfv filtered_value = STFV_INIT;
        size_t last = 0;

        bool start_chunk = true;

        while ((chunk_size = voice_file.read_next_chunk(buffer.get())) != 0) {
            wav_compressing_utils::compress_smooth_amp_chunk(
                    buffer.get(), chunk_size, start_chunk, &smooth_region_end,
                    decibel_utils::spl_to_amp(threshold), silence_samples,
                    &filtered_value, &last, &cdata);
            if (start_chunk) {
                start_chunk = false;
            }
        }

        if (last > 0) {
            if (smooth_region_end) {
                cdata.compressed_data.back() += last;
            } else {
                cdata.compressed_data.push_back(last);
            }
        }
    }

    size_t total_samples = 0;

    for (auto i : cdata.compressed_data) {
        total_samples += i;
    }

    printf("Total samples: %llu, Total seconds: %.3lf\n",
           total_samples,
           total_samples / (double) voice_file.header.sample_rate);

    std::vector<double> coefficients(total_samples);

    total_samples = 0;

    auto current_value = (size_t) cdata.start_value;
    size_t start = 0;

    auto fade_in_len = (size_t) (attack * voice_file.header.sample_rate);
    auto fade_out_len = (size_t) (release * voice_file.header.sample_rate);

    size_t end = cdata.compressed_data.size() - 1;

    if (current_value == 0) {
        signal_fading_utils::fade_expand_start(
                coefficients, fade_in_len,
                cdata.compressed_data[0], gain);
        total_samples += cdata.compressed_data[0];
        current_value = 1;
        start = 1;
    }

    double target = decibel_utils::spl_to_ratio(-fabs(gain));

    for (size_t i = start; i <= end; i++) {
        if (current_value == 1) {
            coefficients.insert(coefficients.end(), cdata.compressed_data[i], target);
        } else if (i == end) {
            signal_fading_utils::fade_expand_end(
                    coefficients, fade_out_len,
                    cdata.compressed_data[i], gain);

        } else {
            signal_fading_utils::fade_expand_normal(
                    coefficients, fade_in_len, fade_out_len,
                    cdata.compressed_data[i], gain);
        }

        total_samples += cdata.compressed_data[i];
        current_value = (current_value == 1 ? 0 : 1);
    }

    cdata.compressed_data.clear();
    coefficients.shrink_to_fit();
    voice_file.refresh();
    orig_file.refresh();

    auto iterator = coefficients.begin();
    std::shared_ptr<Amp[]> voice_buffer(new Amp[voice_file.buffer_size]);
    std::shared_ptr<Amp[]> orig_buffer(new Amp[orig_file.buffer_size]);
    std::shared_ptr<Amp[]> out_buffer(new Amp[output_file.buffer_size]);

    size_t voice_buffer_size;
    size_t back_buffer_size;

//    size_t chunks_read = 0;

    while ((voice_buffer_size = voice_file.read_next_chunk(voice_buffer.get())) != 0
           && (back_buffer_size = orig_file.read_next_chunk(orig_buffer.get())) != 0) {
        end = back_buffer_size / 2;
        if (back_buffer_size != output_file.buffer_size) {
            output_file.buffer_size = back_buffer_size;
        }

        for (size_t i = 0; i < end; i++) {
            if (iterator == coefficients.end()) {
                break;
            }
            out_buffer[i * 2] = (int16_t)((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                          orig_buffer[i * 2] * *iterator);
            out_buffer[i * 2 + 1] = (int16_t)((i < voice_buffer_size ? voice_buffer[i] : 0) +
                                              orig_buffer[i * 2 + 1] * *iterator);
            iterator++;
        }
        output_file.write_next_chunk(out_buffer.get(), back_buffer_size);
    }
}

#undef MODULE_API_EXPORTS

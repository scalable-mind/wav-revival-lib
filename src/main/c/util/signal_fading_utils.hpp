#pragma once

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "decibel_utils.hpp"

namespace signal_fading_utils {
    size_t subtract(size_t a, size_t b) {
        return (a < b ? b - a : a - b);
    }

    void fade_in(std::vector<double> &dst, size_t start, size_t end, size_t sample_count, Decibel gain) {
        double target = decibel_utils::spl_to_ratio(-fabs(gain));
        for (uint64_t i = start; i < end; i++) {
            dst.push_back(1 + (target - 1) * (double) i / sample_count);
        }
    }

    void fade_out(std::vector<double> &dst, size_t start, size_t end, size_t sample_count, Decibel gain) {
        double target = decibel_utils::spl_to_ratio(-fabs(gain));
        for (uint64_t i = start; i < end; i++) {
            dst.push_back(target + (1 - target) * (double) i / sample_count);
        }
    }

    void fade_expand_start(std::vector<double> &dst, size_t fade_in_len, size_t silence_len, Decibel gain) {
        dst.insert(dst.end(), subtract(silence_len, fade_in_len), 1);
        fade_in(dst, 0, fade_in_len, fade_in_len, gain);
    }

    void fade_expand_normal(std::vector<double> &dst, size_t fade_in_len, size_t fade_out_len,
                            size_t silence_len, Decibel gain) {
        fade_out(dst, 0, fade_out_len, fade_out_len, gain);
        dst.insert(dst.end(), subtract(silence_len, (fade_in_len + fade_out_len)), 1);
        fade_in(dst, 0, fade_in_len, fade_in_len, gain);
    }

    void fade_expand_end(std::vector<double> &dst, size_t fade_out_len, size_t silence_len, Decibel gain) {
        if (fade_out_len > silence_len) {
            fade_out(dst, 0, silence_len, fade_out_len, gain);
        } else {
            fade_out(dst, 0, fade_out_len, fade_out_len, gain);
            dst.insert(dst.end(), subtract(silence_len, fade_out_len), 1);
        }
    }
} // namespace signal_fading_utils

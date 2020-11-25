#pragma once

#include <cstdint>
#include <cmath>

/**
 * PCM WAVE 16 Bit INT positive quantization limit.
 * NOTE: using "double" for more precise calculations.
 */
static constexpr double QUANT_LIM = INT16_MAX;

/** Signal amplitude */
typedef int16_t Amp;

typedef double Decibel;

namespace decibel_utils {
    static Decibel get_spl(Amp val) {
        return 20 * log10(val / (val < 0 ? -QUANT_LIM - 1 : QUANT_LIM));
    }

    static Amp spl_to_amp(Decibel gain) {
        return (Amp) (QUANT_LIM * pow(10, 0.05 * gain));
    }

    static double spl_to_ratio(Decibel gain) {
        return pow(10, 0.05 * gain);
    }
} // namespace decibel_utils

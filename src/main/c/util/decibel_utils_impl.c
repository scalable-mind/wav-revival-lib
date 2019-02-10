#include <math.h>

#define MODULE_API_EXPORTS

#include <util/decibel_utils.h>

/**
 * PCM WAVE 16 Bit INT positive quantization limit.
 * NOTE: using "double" for more precise calculations.
 */
static const double QUANT_LIM = INT16_MAX;

static Decibel get_spl(Amp val) {
    return 20 * log10(val / (val < 0 ? -QUANT_LIM - 1 : QUANT_LIM));
}

static Amp spl_to_rsp(Decibel gain) {
    return (Amp) (QUANT_LIM * pow(10, 0.05 * gain));
}

static double spl_to_ratio(Decibel gain) {
    return pow(10, 0.05 * gain);
}

DecibelUtils* decibel_utils() {
    static DecibelUtils instance = { ._is_initialized=false };

    if(!instance._is_initialized) {
        instance._is_initialized = true;
        instance.get_spl = get_spl;
        instance.spl_to_rsp = spl_to_rsp;
        instance.spl_to_ratio = spl_to_ratio;
    }

    return &instance;
}

#undef MODULE_API_EXPORTS

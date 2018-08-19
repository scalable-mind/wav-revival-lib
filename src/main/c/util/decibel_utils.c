#include <math.h>

#include <util/decibel_utils.h>

static double threshold(BitDepth d) {
    switch (d) {
        case BIT_8:
            return 127.0;
        case BIT_16:
            return 32767.0;
        case BIT_24:
            return 8388607.0;
        case BIT_32:
            return 1.0;
        default:
            return -1.0;
    }
}

static double ratio(int16_t v) {
    double tmp = threshold(sizeof(int16_t));
    return 20 * log10(v / (v < 0 ? -tmp - 1 : tmp));
}

static int16_t to_val(double v) {
    double thr = threshold(sizeof(int16_t));
    return (int16_t) (thr * pow(10, 0.05 * v));
}

static double coefficient(double v) {
    return pow(10, 0.05 * v);
}

static bool greater(int16_t src, double threshold) {
    return ratio(src) > threshold;
}

DecibelUtils* decibel_utils() {
    static DecibelUtils instance = { ._is_initialized=false };

    if(!instance._is_initialized) {
        instance._is_initialized = true;
        instance.threshold = threshold;
        instance.greater = greater;
        instance.ratio = ratio;
        instance.to_val = to_val;
        instance.coefficient = coefficient;
    }

    return &instance;
}
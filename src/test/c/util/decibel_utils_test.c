#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <util/decibel_utils.h>

void DecibelUtils__threshold_test_1() {
    typedef struct {
        char data[3];
    } int24_t;
    assert((BitDepth) (sizeof(int8_t)) == BIT_8);
    assert((BitDepth) (sizeof(int16_t)) == BIT_16);
    assert((BitDepth) (sizeof(int24_t)) == BIT_24);
    assert((BitDepth) (sizeof(float)) == BIT_32);
    assert(decibel_utils()->threshold((BitDepth) (sizeof(int8_t))) == 127.0);
    assert(decibel_utils()->threshold((BitDepth) (sizeof(int16_t))) == 32767.0);
    assert(decibel_utils()->threshold((BitDepth) (sizeof(int24_t))) == 8388607.0);
    assert(decibel_utils()->threshold((BitDepth) (sizeof(float))) == 1.0);
    assert(decibel_utils()->threshold((BitDepth) (sizeof(double))) == -1.0);
}

void DecibelUtils__ratio_test_1() {
    const int16_t SIGNAL_AMPS[10] = {-16384, 16383, -23197, 23197, 8230,
                                     -8230,  -4125, 4125,   2067,  -2067};
    const double SIGNAL_DBS[10] = {-6.0206,    -6.020865, -3.000362,  -3.000097,  -12.000737,
                                   -12.001002, -18.00052, -18.000255, -24.001924, -24.002189};
    for (int i = 0; i < 10; ++i) {
        assert(round(decibel_utils()->ratio(SIGNAL_AMPS[i]) * 1000000) / 1000000 == SIGNAL_DBS[i]);
    }
}

void DecibelUtils__to_val_test_1() {
    assert(decibel_utils()->to_val(-3) == 23197);
    assert(decibel_utils()->to_val(-6) == 16422);
    assert(decibel_utils()->to_val(-9) == 11626);
    assert(decibel_utils()->to_val(-12) == 8230);
    assert(decibel_utils()->to_val(-15) == 5826);
    assert(decibel_utils()->to_val(-18) == 4125);
    assert(decibel_utils()->to_val(-21) == 2920);
    assert(decibel_utils()->to_val(-24) == 2067);
}

void DecibelUtils__coefficient_test_1() {
    assert(round(decibel_utils()->coefficient(-3)  * 1000000) / 1000000 == 0.707946);
    assert(round(decibel_utils()->coefficient(-6)  * 1000000) / 1000000 == 0.501187);
    assert(round(decibel_utils()->coefficient(-9)  * 1000000) / 1000000 == 0.354813);
    assert(round(decibel_utils()->coefficient(-12) * 1000000) / 1000000 == 0.251189);
    assert(round(decibel_utils()->coefficient(-15) * 1000000) / 1000000 == 0.177828);
    assert(round(decibel_utils()->coefficient(-18) * 1000000) / 1000000 == 0.125893);
    assert(round(decibel_utils()->coefficient(-21) * 1000000) / 1000000 == 0.089125);
    assert(round(decibel_utils()->coefficient(-24) * 1000000) / 1000000 == 0.063096);
}

void DecibelUtils__greater_test_1() {
    const int16_t SIGNAL_AMPS[14] = {16420,  16421,  16422,  16423,  16424,  -16420, -16421,
                                     -16422, -16423, -16424, -32766, -32768, 32766,  32767};
    const bool RESULTS[14] = {false, false, false, true,  true,  false, false,
                              false, true, true, false, false, false, false};
    int i = 0;
    for(; i < 10; ++i) {
        assert(decibel_utils()->greater(SIGNAL_AMPS[i], -6) == RESULTS[i]);
    }
    for (; i < 14; ++i) {
        assert(decibel_utils()->greater(SIGNAL_AMPS[i], 0) == RESULTS[i]);
    }
}

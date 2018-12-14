#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <util/decibel_utils.h>

void DecibelUtils__get_spl__test_1() {
    const Rsp SIGNAL_AMPS[10] = {-16384, 16383, -23197, 23197, 8230,
                                     -8230,  -4125, 4125,   2067,  -2067};
    const Decibel SIGNAL_DBS[10] = {-6.0206,    -6.020865, -3.000362,  -3.000097,  -12.000737,
                                   -12.001002, -18.00052, -18.000255, -24.001924, -24.002189};
    for (int i = 0; i < 10; ++i) {
        assert(round(decibel_utils()->get_spl(SIGNAL_AMPS[i]) * 1000000) / 1000000 == SIGNAL_DBS[i]);
    }
}

void DecibelUtils__spl_to_pascal__test_1() {
    assert(decibel_utils()->spl_to_rsp(-3) == 23197);
    assert(decibel_utils()->spl_to_rsp(-6) == 16422);
    assert(decibel_utils()->spl_to_rsp(-9) == 11626);
    assert(decibel_utils()->spl_to_rsp(-12) == 8230);
    assert(decibel_utils()->spl_to_rsp(-15) == 5826);
    assert(decibel_utils()->spl_to_rsp(-18) == 4125);
    assert(decibel_utils()->spl_to_rsp(-21) == 2920);
    assert(decibel_utils()->spl_to_rsp(-24) == 2067);
}

void DecibelUtils__spl_to_ratio__test_1() {
    assert(round(decibel_utils()->spl_to_ratio(-3)  * 1000000) / 1000000 == 0.707946);
    assert(round(decibel_utils()->spl_to_ratio(-6)  * 1000000) / 1000000 == 0.501187);
    assert(round(decibel_utils()->spl_to_ratio(-9)  * 1000000) / 1000000 == 0.354813);
    assert(round(decibel_utils()->spl_to_ratio(-12) * 1000000) / 1000000 == 0.251189);
    assert(round(decibel_utils()->spl_to_ratio(-15) * 1000000) / 1000000 == 0.177828);
    assert(round(decibel_utils()->spl_to_ratio(-18) * 1000000) / 1000000 == 0.125893);
    assert(round(decibel_utils()->spl_to_ratio(-21) * 1000000) / 1000000 == 0.089125);
    assert(round(decibel_utils()->spl_to_ratio(-24) * 1000000) / 1000000 == 0.063096);
}

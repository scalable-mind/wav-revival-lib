#include <cstdint>
#include <cmath>

#include <catch2/catch.hpp>

#include <util/decibel_utils.h>

TEST_CASE("DecibelUtils::get_spl - test #1", "[DecibelUtils]") {
    const Amp SIGNAL_AMPS[10] = {-16384, 16383, -23197, 23197, 8230,
                                     -8230,  -4125, 4125,   2067,  -2067};
    const Decibel SIGNAL_DBS[10] = {-6.0206,    -6.020865, -3.000362,  -3.000097,  -12.000737,
                                   -12.001002, -18.00052, -18.000255, -24.001924, -24.002189};
    for (int i = 0; i < 10; ++i) {
        REQUIRE(round(decibel_utils()->get_spl(SIGNAL_AMPS[i]) * 1000000) / 1000000 == SIGNAL_DBS[i]);
    }
}

TEST_CASE("DecibelUtils::spl_to_rsp - test #1", "[DecibelUtils]") {
    REQUIRE(decibel_utils()->spl_to_rsp(-3) == 23197);
    REQUIRE(decibel_utils()->spl_to_rsp(-6) == 16422);
    REQUIRE(decibel_utils()->spl_to_rsp(-9) == 11626);
    REQUIRE(decibel_utils()->spl_to_rsp(-12) == 8230);
    REQUIRE(decibel_utils()->spl_to_rsp(-15) == 5826);
    REQUIRE(decibel_utils()->spl_to_rsp(-18) == 4125);
    REQUIRE(decibel_utils()->spl_to_rsp(-21) == 2920);
    REQUIRE(decibel_utils()->spl_to_rsp(-24) == 2067);
}

TEST_CASE("DecibelUtils::spl_to_ratio - test #1", "[DecibelUtils]") {
    REQUIRE(round(decibel_utils()->spl_to_ratio(-3)  * 1000000) / 1000000 == 0.707946);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-6)  * 1000000) / 1000000 == 0.501187);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-9)  * 1000000) / 1000000 == 0.354813);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-12) * 1000000) / 1000000 == 0.251189);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-15) * 1000000) / 1000000 == 0.177828);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-18) * 1000000) / 1000000 == 0.125893);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-21) * 1000000) / 1000000 == 0.089125);
    REQUIRE(round(decibel_utils()->spl_to_ratio(-24) * 1000000) / 1000000 == 0.063096);
}

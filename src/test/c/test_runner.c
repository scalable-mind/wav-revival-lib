#include <util/decibel_utils_test.h>
#include <util/signal_fading_utils_test.h>
#include <util/wav_compressing_utils_test.h>
#include <api/double_array_api_impl_test.h>
#include <api/uint64_array_api_impl_test.h>
#include <tiny_test/test_utils.h>

int main(int argc, char* argv[]) {
    // DecibelUtils Tests
    ADD_TEST(DecibelUtils__get_spl__test_1);
    ADD_TEST(DecibelUtils__spl_to_pascal__test_1);
    ADD_TEST(DecibelUtils__spl_to_ratio__test_1);

    // DoubleArrayApiImpl Tests
    ADD_TEST(DoubleArrayApiImpl__push_back__test_1);
    ADD_TEST(DoubleArrayApiImpl__shrink_to_fit__test_1);
    ADD_TEST(DoubleArrayApiImpl__shrink_to_fit__test_2);
    ADD_TEST(DoubleArrayApiImpl__push_back__test_2);
    ADD_TEST(DoubleArrayApiImpl__push_some__test_1);
    ADD_TEST(DoubleArrayApiImpl__end__test_1);

    // SignalFadingUtils Tests
    ADD_TEST(SignalFadingUtils__fade_in__test_1);
    ADD_TEST(SignalFadingUtils__fade_out__test_1);
    ADD_TEST(SignalFadingUtils__fade_expand__test_1);
    ADD_TEST(SignalFadingUtils__fade_expand__test_2);

    // WavCompressingUtils Tests
    ADD_TEST(WavCompressingUtils__compress_int16_array__test_1);
    ADD_TEST(WavCompressingUtils__compress_smooth_int16_array__test_1);
    ADD_TEST(WavCompressingUtils__compress_smooth_int16_array__test_2);
    ADD_TEST(WavCompressingUtils__compress_smooth_int16_array__test_3);

    // Uint64ArrayApiImpl Tests
    ADD_TEST(Uint64ArrayApiImpl__push_back__test_1);
    ADD_TEST(Uint64ArrayApiImpl__shrink_to_fit__test_1);
    ADD_TEST(Uint64ArrayApiImpl__push_back__test_2);
    ADD_TEST(Uint64ArrayApiImpl__end__test_1);

    test_utils()->run_tests();
}

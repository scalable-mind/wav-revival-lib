#include <cmath>

#include <catch2/catch.hpp>

#include <util/decibel_utils.hpp>
#include <util/signal_fading_utils.hpp>

TEST_CASE("SignalFadingUtils::fade_in - test #1", "[SignalFadingUtils]") {
    std::vector<double> arr;
    signal_fading_utils::fade_in(arr, 0, 5, 5, 6);
    double target = decibel_utils::spl_to_ratio(-fabs(6.0));
    double expected = 1 + (target - 1) * 4 / 5;
    double actual = arr.back();
    REQUIRE(actual == expected);
}

TEST_CASE("SignalFadingUtils::fade_out - test #1", "[SignalFadingUtils]") {
    std::vector<double> arr;
    signal_fading_utils::fade_out(arr, 0, 5, 5, 6);
    double target = decibel_utils::spl_to_ratio(-fabs(6.0));
    double expected = target + (1 - target) * 4 / 5;
    double actual = arr.back();
    REQUIRE(actual == expected);
}

#define EXPECTED_SIZE 40

TEST_CASE("SignalFadingUtils::fade_expand - test #1", "[SignalFadingUtils]") {
    bool arrays_equal = true;

    const double expected[EXPECTED_SIZE] = {1, 1, 1,
                                            1, 0.900237, 0.800475, 0.700712, 0.600950,
                                            0.501187, 0.501187, 0.501187, 0.501187,
                                            0.501187, 0.600950, 0.700712, 0.800475, 0.900237,
                                            1, 1, 1, 1,
                                            1, 0.900237, 0.800475, 0.700712, 0.600950,
                                            0.501187, 0.501187, 0.501187, 0.501187,
                                            0.501187, 0.600950, 0.700712, 0.800475, 0.900237,
                                            1, 1, 1, 1, 1};

    std::vector<double> actual;

    double factor = decibel_utils::spl_to_ratio(-fabs(6.0));

    // Working with actual to compare with expected
    signal_fading_utils::fade_expand_start(actual, 5, 8, 6);
    actual.insert(actual.end(), 4, factor);
    signal_fading_utils::fade_expand_normal(actual, 5, 5, 14, 6);
    actual.insert(actual.end(), 4, factor);
    signal_fading_utils::fade_expand_end(actual, 5, 10, 6);

    for (int i = 0; i < EXPECTED_SIZE; ++i) {
        arrays_equal = arrays_equal && (round(actual[i] * 1000000) / 1000000 == expected[i]);
    }

    size_t actual_size = actual.size();

    REQUIRE(actual_size == EXPECTED_SIZE);
    REQUIRE(arrays_equal);

}

TEST_CASE("SignalFadingUtils__fade_expand__test_2", "[SignalFadingUtils]") {
#undef EXPECTED_SIZE
#define EXPECTED_SIZE 34
    bool arrays_equal = true;
    const double expected[EXPECTED_SIZE] = {1, 1, 1,
                                            1, 0.900237, 0.800475, 0.700712, 0.600950,
                                            0.501187, 0.501187, 0.501187, 0.501187,
                                            0.501187, 0.600950, 0.700712, 0.800475, 0.900237,
                                            1, 1, 1, 1,
                                            1, 0.900237, 0.800475, 0.700712, 0.600950,
                                            0.501187, 0.501187, 0.501187, 0.501187,
                                            0.501187, 0.600950, 0.700712, 0.800475};

    std::vector<double> actual;

    double factor = decibel_utils::spl_to_ratio(-fabs(6.0));

    // Working with actual to compare with expected
    signal_fading_utils::fade_expand_start(actual, 5, 8, 6);
    actual.insert(actual.end(), 4, factor);
    signal_fading_utils::fade_expand_normal(actual, 5, 5, 14, 6);
    actual.insert(actual.end(), 4, factor);
    signal_fading_utils::fade_expand_end(actual, 5, 4, 6);

    for (int i = 0; i < EXPECTED_SIZE; ++i) {
        arrays_equal = arrays_equal && (round(actual[i] * 1000000) / 1000000 == expected[i]);
    }

    size_t actual_size = actual.size();

    REQUIRE(actual_size == EXPECTED_SIZE);
    REQUIRE(arrays_equal);
}

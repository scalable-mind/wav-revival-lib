#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <api/double_array_api.h>
#include <util/decibel_utils.h>
#include <util/signal_fading_utils.h>
#include <domain/double_array.h>

void SignalFadingUtils__fade_in__test_1() {
    DoubleArray* arr = double_array_api()->init(0);
    signal_fading_utils()->fade_in(arr, 0, 5, 5, 6);
    double target = decibel_utils()->spl_to_ratio(-fabs(6));
    double expected = 1 + (target - 1) * 4 / 5;
    double actual = arr->data[arr->size - 1];
    double_array_api()->del(arr);
    assert(actual == expected);
}

void SignalFadingUtils__fade_out__test_1() {
    DoubleArray* arr = double_array_api()->init(0);
    signal_fading_utils()->fade_out(arr, 0, 5, 5, 6);
    double target = decibel_utils()->spl_to_ratio(-fabs(6));
    double expected = target + (1 - target) * 4 / 5;
    double actual = arr->data[arr->size - 1];
    double_array_api()->del(arr);
    assert(actual == expected);
}

#define EXPECTED_SIZE 40

void SignalFadingUtils__fade_expand__test_1() {
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

    DoubleArray* actual = double_array_api()->init(0);

    double factor = decibel_utils()->spl_to_ratio(-fabs(6));

    // Working with actual to compare with expected
    signal_fading_utils()->fade_expand_start(actual, 5, 8, 6);
    double_array_api()->push_some(actual, 4, factor);
    signal_fading_utils()->fade_expand_normal(actual, 5, 5, 14, 6);
    double_array_api()->push_some(actual, 4, factor);
    signal_fading_utils()->fade_expand_end(actual, 5, 10, 6);

    for (int i = 0; i < EXPECTED_SIZE; ++i) {
        arrays_equal = arrays_equal && (round(actual->data[i] * 1000000) / 1000000 == expected[i]);
    }

    size_t actual_size = actual->size;

    double_array_api()->del(actual);

    assert(actual_size == EXPECTED_SIZE);
    assert(arrays_equal == true);

}

#undef EXPECTED_SIZE

#define EXPECTED_SIZE 34

void SignalFadingUtils__fade_expand__test_2() {
    bool arrays_equal = true;
    const double expected[EXPECTED_SIZE] = {1, 1, 1,
                                            1, 0.900237, 0.800475, 0.700712, 0.600950,
                                            0.501187, 0.501187, 0.501187, 0.501187,
                                            0.501187, 0.600950, 0.700712, 0.800475, 0.900237,
                                            1, 1, 1, 1,
                                            1, 0.900237, 0.800475, 0.700712, 0.600950,
                                            0.501187, 0.501187, 0.501187, 0.501187,
                                            0.501187, 0.600950, 0.700712, 0.800475};

    DoubleArray* actual = double_array_api()->init(0);

    double factor = decibel_utils()->spl_to_ratio(-fabs(6));

    // Working with actual to compare with expected
    signal_fading_utils()->fade_expand_start(actual, 5, 8, 6);
    double_array_api()->push_some(actual, 4, factor);
    signal_fading_utils()->fade_expand_normal(actual, 5, 5, 14, 6);
    double_array_api()->push_some(actual, 4, factor);
    signal_fading_utils()->fade_expand_end(actual, 5, 4, 6);

    for (int i = 0; i < EXPECTED_SIZE; ++i) {
        arrays_equal = arrays_equal && (round(actual->data[i] * 1000000) / 1000000 == expected[i]);
    }

    size_t actual_size = actual->size;

    double_array_api()->del(actual);

    assert(actual_size == EXPECTED_SIZE);
    assert(arrays_equal == true);
}

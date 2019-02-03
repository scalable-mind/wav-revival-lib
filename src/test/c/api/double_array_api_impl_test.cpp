#include <catch2/catch.hpp>

#include <api/double_array_api.h>
#include <domain/double_array.h>


TEST_CASE("DoubleArrayApiImpl::push_back - test #1", "[DoubleArrayApiImpl]") {
    DoubleArray* arr = double_array_api()->init(1);

    size_t actual_size_at_start = arr->_size;
    double_array_api()->push_back(arr, 5555);
    size_t actual_size_afterwards = arr->_size;

    double_array_api()->del(arr);

    REQUIRE(actual_size_at_start == 0);
    REQUIRE(actual_size_afterwards == 1);
}


TEST_CASE("DoubleArrayApiImpl::shrink_to_fit - test #1", "[DoubleArrayApiImpl]") {
#define EXPECTED_CAPACITY 100000
    DoubleArray* arr = double_array_api()->init(EXPECTED_CAPACITY * 100);
    for (int i = 0; i < EXPECTED_CAPACITY; i++) {
        double_array_api()->push_back(arr, 5555);
    }

    double_array_api()->shrink_to_fit(arr);
    size_t actual_capacity = arr->_capacity;

    double_array_api()->del(arr);

    REQUIRE(actual_capacity == EXPECTED_CAPACITY);
#undef EXPECTED_CAPACITY
}


TEST_CASE("DoubleArrayApiImpl::shrink_to_fit - test #2", "[DoubleArrayApiImpl]") {
#define EXPECTED_CAPACITY 64
    DoubleArray* arr = double_array_api()->init(1000);
    for (int i = 0; i < EXPECTED_CAPACITY / 2; i++) {
        double_array_api()->push_back(arr, 5555);
    }

    double_array_api()->shrink_to_fit(arr);
    size_t actual_capacity = arr->_capacity;

    double_array_api()->del(arr);

    REQUIRE(actual_capacity == EXPECTED_CAPACITY);
#undef EXPECTED_CAPACITY
}

TEST_CASE("DoubleArrayApiImpl::push_back - test #2", "[DoubleArrayApiImpl]") {
#define CAPACITY_AT_START 10
    DoubleArray* arr = double_array_api()->init(CAPACITY_AT_START);

    for (int i = 0; i < CAPACITY_AT_START; i++) {
        double_array_api()->push_back(arr, 5555);
    }
    double_array_api()->push_back(arr, 8888);  // this pushing triggers the array to resize its capacity
    size_t actual_capacity_afterwards = arr->_capacity;

    double_array_api()->del(arr);

    REQUIRE(actual_capacity_afterwards > CAPACITY_AT_START);
#undef CAPACITY_AT_START
}

TEST_CASE("DoubleArrayApiImpl::push_some - test #1", "[DoubleArrayApiImpl]") {
#define EXPECTED_SIZE 10000000
    DoubleArray* arr = double_array_api()->init(0);

    double_array_api()->push_some(arr, EXPECTED_SIZE, 3.14);

    size_t actual_size_afterwards = arr->_size;

    double_array_api()->del(arr);

    REQUIRE(actual_size_afterwards == EXPECTED_SIZE);
#undef EXPECTED_SIZE
}

TEST_CASE("DoubleArrayApiImpl::end - test #1", "[DoubleArrayApiImpl]") {
#define EXPECTED_END_AT_START 5555
#define EXPECTED_END_AFTERWARDS 8888
    DoubleArray* arr = double_array_api()->init(5);

    double_array_api()->push_back(arr, EXPECTED_END_AT_START);
    double actual_end_at_start = *(double_array_api()->end(arr) - 1);

    double_array_api()->push_back(arr, EXPECTED_END_AFTERWARDS);
    double actual_end_afterwards = *(double_array_api()->end(arr) - 1);

    double_array_api()->del(arr);

    REQUIRE(actual_end_at_start == EXPECTED_END_AT_START);
    REQUIRE(actual_end_afterwards == EXPECTED_END_AFTERWARDS);
#undef EXPECTED_END_AT_START
#undef EXPECTED_END_AFTERWARDS
}

#include <catch2/catch.hpp>

#include <api/uint64_array_api.h>
#include <domain/uint64_array.h>

TEST_CASE("Uint64ArrayApiImpl::push_back - test #1", "[Uint64ArrayApiImpl]") {
    Uint64Array* arr = uint64_array_api()->init(1);

    size_t actual_size_at_start = arr->_size;
    uint64_array_api()->push_back(arr, 5555);
    size_t actual_size_afterwards = arr->_size;

    uint64_array_api()->del(arr);

    REQUIRE(actual_size_at_start == 0);
    REQUIRE(actual_size_afterwards == 1);
}



TEST_CASE("Uint64ArrayApiImpl::shrink_to_fit - test #1", "[Uint64ArrayApiImpl]") {
#define EXPECTED_CAPACITY 64
    Uint64Array* arr = uint64_array_api()->init(100);
    for (int i = 0; i < EXPECTED_CAPACITY / 2; i++) {
        uint64_array_api()->push_back(arr, 5555);
    }

    uint64_array_api()->shrink_to_fit(arr);
    size_t actual_capacity = arr->_capacity;

    uint64_array_api()->del(arr);

    REQUIRE(actual_capacity == EXPECTED_CAPACITY);
#undef EXPECTED_CAPACITY
}


TEST_CASE("Uint64ArrayApiImpl::push_back - test #2", "[Uint64ArrayApiImpl]") {
#define CAPACITY_AT_START 10
    Uint64Array* arr = uint64_array_api()->init(CAPACITY_AT_START);

    for (int i = 0; i < CAPACITY_AT_START; i++) {
        uint64_array_api()->push_back(arr, 5555);
    }
    uint64_array_api()->push_back(arr, 8888);  // this pushing triggers the array to resize its capacity
    size_t actual_capacity_afterwards = arr->_capacity;

    uint64_array_api()->del(arr);

    REQUIRE(actual_capacity_afterwards > CAPACITY_AT_START);
#undef CAPACITY_AT_START
}

TEST_CASE("Uint64ArrayApiImpl::end - test #1", "[Uint64ArrayApiImpl]") {
#define EXPECTED_END_AT_START 5555
#define EXPECTED_END_AFTERWARDS 8888
    Uint64Array* arr = uint64_array_api()->init(5);

    uint64_array_api()->push_back(arr, EXPECTED_END_AT_START);
    uint64_t actual_end_at_start = *(uint64_array_api()->end(arr) - 1);

    uint64_array_api()->push_back(arr, EXPECTED_END_AFTERWARDS);
    uint64_t actual_end_afterwards = *(uint64_array_api()->end(arr) - 1);

    uint64_array_api()->del(arr);

    REQUIRE(actual_end_at_start == EXPECTED_END_AT_START);
    REQUIRE(actual_end_afterwards == EXPECTED_END_AFTERWARDS);
#undef EXPECTED_END_AT_START
#undef EXPECTED_END_AFTERWARDS
}

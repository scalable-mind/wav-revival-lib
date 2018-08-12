#include <assert.h>
#include <stdio.h>

#include <api/uint64_array_api.h>
#include <domain/uint64_array.h>


void Uint64ArrayApiImpl__push_back__test_1() {
    Uint64Array* arr = uint64_array_api().init(1);

    size_t actual_size_at_start = arr->size;
    uint64_array_api().push_back(arr, 5555);
    size_t actual_size_afterwards = arr->size;

    uint64_array_api().del(arr);

    assert(actual_size_at_start == 0);
    assert(actual_size_afterwards == 1);
}


#define EXPECTED_CAPACITY 64
void Uint64ArrayApiImpl__shrink_to_fit__test_1() {
    Uint64Array* arr = uint64_array_api().init(100);
    for (int i = 0; i < EXPECTED_CAPACITY / 2; i++) {
        uint64_array_api().push_back(arr, 5555);
    }

    uint64_array_api().shrink_to_fit(arr);
    size_t actual_capacity = arr->capacity;

    uint64_array_api().del(arr);

    assert(actual_capacity == EXPECTED_CAPACITY);
}
#undef EXPECTED_CAPACITY


#define CAPACITY_AT_START 10
void Uint64ArrayApiImpl__push_back__test_2() {
    Uint64Array* arr = uint64_array_api().init(CAPACITY_AT_START);

    for (int i = 0; i < CAPACITY_AT_START; i++) {
        uint64_array_api().push_back(arr, 5555);
    }
    uint64_array_api().push_back(arr, 8888);  // this pushing triggers the array to resize its capacity
    size_t actual_capacity_afterwards = arr->capacity;

    uint64_array_api().del(arr);

    assert(actual_capacity_afterwards > CAPACITY_AT_START);
}
#undef CAPACITY_AT_START


#define EXPECTED_END_AT_START 5555
#define EXPECTED_END_AFTERWARDS 8888
void Uint64ArrayApiImpl__end__test_1() {
    Uint64Array* arr = uint64_array_api().init(5);

    uint64_array_api().push_back(arr, EXPECTED_END_AT_START);
    uint64_t actual_end_at_start = *(uint64_array_api().end(arr) - 1);

    uint64_array_api().push_back(arr, EXPECTED_END_AFTERWARDS);
    uint64_t actual_end_afterwards = *(uint64_array_api().end(arr) - 1);

    uint64_array_api().del(arr);

    assert(actual_end_at_start == EXPECTED_END_AT_START);
    assert(actual_end_afterwards == EXPECTED_END_AFTERWARDS);
}
#undef EXPECTED_END_AT_START
#undef EXPECTED_END_AFTERWARDS

#include <assert.h>
#include <stdio.h>

#include <api/double_array_api.h>
#include <domain/double_array.h>


void DoubleArrayApiImpl__push_back__test_1() {
    DoubleArray* arr = double_array_api()->init(1);

    size_t actual_size_at_start = arr->size;
    double_array_api()->push_back(arr, 5555);
    size_t actual_size_afterwards = arr->size;

    double_array_api()->del(arr);

    assert(actual_size_at_start == 0);
    assert(actual_size_afterwards == 1);
}


#define EXPECTED_CAPACITY 10000000
void DoubleArrayApiImpl__shrink_to_fit__test_1() {
    DoubleArray* arr = double_array_api()->init(EXPECTED_CAPACITY*100);
    for (int i = 0; i < EXPECTED_CAPACITY; i++) {
        double_array_api()->push_back(arr, 5555);
    }

    double_array_api()->shrink_to_fit(arr);
    size_t actual_capacity = arr->capacity;

    double_array_api()->del(arr);

    assert(actual_capacity == EXPECTED_CAPACITY);
}
#undef EXPECTED_CAPACITY


#define EXPECTED_CAPACITY 64
void DoubleArrayApiImpl__shrink_to_fit__test_2() {
    DoubleArray* arr = double_array_api()->init(1000);
    for (int i = 0; i < EXPECTED_CAPACITY / 2; i++) {
        double_array_api()->push_back(arr, 5555);
    }

    double_array_api()->shrink_to_fit(arr);
    size_t actual_capacity = arr->capacity;

    double_array_api()->del(arr);

    assert(actual_capacity == EXPECTED_CAPACITY);
}
#undef EXPECTED_CAPACITY


#define CAPACITY_AT_START 10
void DoubleArrayApiImpl__push_back__test_2() {
    DoubleArray* arr = double_array_api()->init(CAPACITY_AT_START);

    for (int i = 0; i < CAPACITY_AT_START; i++) {
        double_array_api()->push_back(arr, 5555);
    }
    double_array_api()->push_back(arr, 8888);  // this pushing triggers the array to resize its capacity
    size_t actual_capacity_afterwards = arr->capacity;

    double_array_api()->del(arr);

    assert(actual_capacity_afterwards > CAPACITY_AT_START);
}
#undef CAPACITY_AT_START


#define EXPECTED_SIZE 10000000
void DoubleArrayApiImpl__push_some__test_1() {
    DoubleArray* arr = double_array_api()->init(0);

    double_array_api()->push_some(arr, EXPECTED_SIZE, 3.14);

    size_t actual_size_afterwards = arr->size;

    double_array_api()->del(arr);

    assert(actual_size_afterwards == EXPECTED_SIZE);
}
#undef EXPECTED_SIZE


#define EXPECTED_END_AT_START 5555
#define EXPECTED_END_AFTERWARDS 8888
void DoubleArrayApiImpl__end__test_1() {
    DoubleArray* arr = double_array_api()->init(5);

    double_array_api()->push_back(arr, EXPECTED_END_AT_START);
    double actual_end_at_start = *(double_array_api()->end(arr) - 1);

    double_array_api()->push_back(arr, EXPECTED_END_AFTERWARDS);
    double actual_end_afterwards = *(double_array_api()->end(arr) - 1);

    double_array_api()->del(arr);

    assert(actual_end_at_start == EXPECTED_END_AT_START);
    assert(actual_end_afterwards == EXPECTED_END_AFTERWARDS);
}
#undef EXPECTED_END_AT_START
#undef EXPECTED_END_AFTERWARDS

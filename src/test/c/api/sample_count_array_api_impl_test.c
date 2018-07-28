#include <assert.h>
#include <stdio.h>

#include <api/sample_count_array_api.h>
#include <domain/sample_count_array.h>


void SampleCountArrayApiImpl__push_back__test_1() {
    SampleCountArray* sca = sample_count_array_api().init(1);

    size_t actual_size_at_start = sca->size;
    sample_count_array_api().push_back(sca, 5555);
    size_t actual_size_afterwards = sca->size;

    sample_count_array_api().del(sca);

    assert(actual_size_at_start == 0);
    assert(actual_size_afterwards == 1);
}


#define EXPECTED_CAPACITY 56
void SampleCountArrayApiImpl__shrink_to_fit__test_1() {
    SampleCountArray* sca = sample_count_array_api().init(100);
    for (int i = 0; i < EXPECTED_CAPACITY; i++) {
        sample_count_array_api().push_back(sca, 5555);
    }

    sample_count_array_api().shrink_to_fit(sca);
    size_t actual_capacity = sca->capacity;

    sample_count_array_api().del(sca);

    assert(actual_capacity == EXPECTED_CAPACITY);
}
#undef EXPECTED_CAPACITY


#define CAPACITY_AT_START 10
void SampleCountArrayApiImpl__push_back__test_2() {
    SampleCountArray* sca = sample_count_array_api().init(CAPACITY_AT_START);

    for (int i = 0; i < CAPACITY_AT_START; i++) {
        sample_count_array_api().push_back(sca, 5555);
    }
    sample_count_array_api().push_back(sca, 8888);  // this pushing triggers the array to resize its capacity
    size_t actual_capacity_afterwards = sca->capacity;

    sample_count_array_api().del(sca);

    assert(actual_capacity_afterwards > CAPACITY_AT_START);
}
#undef CAPACITY_AT_START


#define EXPECTED_END_AT_START 5555
#define EXPECTED_END_AFTERWARDS 8888
void SampleCountArrayApiImpl__end__test_1() {
    SampleCountArray* sca = sample_count_array_api().init(5);

    sample_count_array_api().push_back(sca, EXPECTED_END_AT_START);
    size_t actual_end_at_start = *(sample_count_array_api().end(sca) - 1);

    sample_count_array_api().push_back(sca, EXPECTED_END_AFTERWARDS);
    size_t actual_end_afterwards = *(sample_count_array_api().end(sca) - 1);

    sample_count_array_api().del(sca);

    assert(actual_end_at_start == EXPECTED_END_AT_START);
    assert(actual_end_afterwards == EXPECTED_END_AFTERWARDS);
}
#undef EXPECTED_END_AT_START
#undef EXPECTED_END_AFTERWARDS

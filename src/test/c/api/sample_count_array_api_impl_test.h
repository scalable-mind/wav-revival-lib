#ifndef SAMPLE_COUNT_ARRAY_API_IMPL_TEST_H
#define SAMPLE_COUNT_ARRAY_API_IMPL_TEST_H

/*
 * Checks if the size has been incremented after pushing.
 */
void SampleCountArrayApiImpl__push_back__test_1();

/*
 * Checks if it is possible to shrink the capacity without resizing.
 */
void SampleCountArrayApiImpl__shrink_to_fit__test_1();

/*
 * Checks if the capacity has been increased after pushing.
 */
void SampleCountArrayApiImpl__push_back__test_2();

/*
 * Checks if `end` always points to the last element + 1 position.
 */
void SampleCountArrayApiImpl__end__test_1();

#endif

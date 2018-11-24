#ifndef DOUBLE_ARRAY_API_IMPL_TEST_H
#define DOUBLE_ARRAY_API_IMPL_TEST_H

/*
 * Checks if the size has been incremented after pushing.
 */
void DoubleArrayApiImpl__push_back__test_1();

/*
 * Checks if it is possible to shrink the capacity without resizing.
 */
void DoubleArrayApiImpl__shrink_to_fit__test_1();

void DoubleArrayApiImpl__shrink_to_fit__test_2();

/*
 * Checks if the capacity has been increased after pushing.
 */
void DoubleArrayApiImpl__push_back__test_2();

/*
 * Checks if the size has been increased after pushing n elements.
 */
void DoubleArrayApiImpl__push_some__test_3();

/*
 * Checks if `end` always points to the last element + 1 position.
 */
void DoubleArrayApiImpl__end__test_1();

#endif

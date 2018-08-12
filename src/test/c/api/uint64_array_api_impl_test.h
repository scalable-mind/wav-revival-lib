#ifndef UINT64_ARRAY_API_IMPL_TEST_H
#define UINT64_ARRAY_API_IMPL_TEST_H

/*
 * Checks if the size has been incremented after pushing.
 */
void Uint64ArrayApiImpl__push_back__test_1();

/*
 * Checks if it is possible to shrink the capacity without resizing.
 */
void Uint64ArrayApiImpl__shrink_to_fit__test_1();

/*
 * Checks if the capacity has been increased after pushing.
 */
void Uint64ArrayApiImpl__push_back__test_2();

/*
 * Checks if `end` always points to the last element + 1 position.
 */
void Uint64ArrayApiImpl__end__test_1();

#endif

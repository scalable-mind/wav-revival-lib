#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdint.h>

#define add_test(test_function) _add_test(#test_function, test_function)
#define TEST_NAME_MAX_LENGTH 256

typedef void (*TestFunction)();

typedef struct {

    char _name[TEST_NAME_MAX_LENGTH];

    TestFunction _function;

} _Test;

typedef struct {

    _Test* _tests_queue;

    size_t _tests_queue_size;

    void (*_add_test)(const char* name, TestFunction function);

    void (*run_tests)();

} TestUtils;

TestUtils* test_utils();

#endif

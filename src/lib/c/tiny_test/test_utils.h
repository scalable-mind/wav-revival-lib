#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#define ADD_TEST(test_function) test_utils()->add_test(#test_function, test_function)
#define TEST_NAME_MAX_LENGTH 256

typedef void (*TestFunction)();

typedef struct {

    char _name[TEST_NAME_MAX_LENGTH];

    TestFunction _function;

} _Test;

typedef struct {

    bool _is_initialized;

    _Test* _tests_queue;

    size_t _tests_queue_size;

    void (*add_test)(const char* name, TestFunction function);

    void (*run_tests)();

} TestUtils;

TestUtils* test_utils();

#endif

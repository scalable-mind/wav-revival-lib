#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tiny_test/test_utils.h>

static void add_test(const char* name, TestFunction function) {
    _Test test;
    strncpy_s(test._name, TEST_NAME_MAX_LENGTH, name, _TRUNCATE);
    test._function = function;

    if (test_utils()->_tests_queue_size == 0) {
        test_utils()->_tests_queue = calloc(1, sizeof(_Test));
        test_utils()->_tests_queue[0] = test;
        test_utils()->_tests_queue_size++;
    } else {
        _Test* new_tests_queue = calloc(test_utils()->_tests_queue_size + 1, sizeof(_Test));
        memcpy(new_tests_queue, test_utils()->_tests_queue, test_utils()->_tests_queue_size * sizeof(_Test));
        new_tests_queue[test_utils()->_tests_queue_size++] = test;
        free(test_utils()->_tests_queue);
        test_utils()->_tests_queue = new_tests_queue;
    }
}

static void run_tests() {
    clock_t start_all_clock = clock();
    for (int i = 0; i < test_utils()->_tests_queue_size; i++) {
        clock_t start_clock = clock();
        test_utils()->_tests_queue[i]._function();
        clock_t end_clock = clock();
        printf("SUCCESS: `%s` [%.3lf sec]\n", test_utils()->_tests_queue[i]._name,
               ((double) end_clock - start_clock) / CLOCKS_PER_SEC);
    }
    clock_t end_all_clock = clock();

    printf("\n%d tests passed [%.3lf sec total]\n", (int) test_utils()->_tests_queue_size,
           ((double) end_all_clock - start_all_clock) / CLOCKS_PER_SEC);

    free(test_utils()->_tests_queue);
}

TestUtils* test_utils() {
    static TestUtils instance;
    static bool is_initialized = false;

    if (!is_initialized) {
        is_initialized = true;
        instance._tests_queue_size = 0;
        instance.add_test = add_test;
        instance.run_tests = run_tests;
    }
    return &instance;
}

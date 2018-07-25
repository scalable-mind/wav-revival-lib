# wav-revival-lib

## How to test

The *test function* is a noreturn function without parameters (a procedure):

```c
void MyFirstApiImpl__my_first_method__test() {
    // ...
}
```

For assertions you could use the `assert` macro from `<assert.h>`:

```c
void MyFirstApiImpl__my_first_method__test() {
    // ...
    assert(actual == EXPECTED);
}
```

To run your tests, use the `TestUtils.add_test` method to add all the test functions you have (need), and then call
`TestUtils.run_tests`:

```c
#include "./api/my_first_api_impl_test.h"
#include "./api/my_second_api_impl_test.h"
#include "./lib/test_utils.h"

int main() {
    test_utils()->add_test(MyFirstApiImpl__my_first_method__test);
    test_utils()->add_test(MyFirstApiImpl__my_second_method__test);
    test_utils()->add_test(MyFirstApiImpl__my_third_method__test);

    test_utils()->add_test(MySecondApiImpl__my_first_method__test);
    test_utils()->add_test(MySecondApiImpl__my_second_method__test);
    test_utils()->add_test(MySecondApiImpl__my_third_method__test);

    test_utils()->run_tests();
}
```

Build with `src/test/c/lib/test_utils.c`.

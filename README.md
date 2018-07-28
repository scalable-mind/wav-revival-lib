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

> NOTE: It is highly recommended to free all the resources in your tests before the `assert` statements.

To run your tests, use the `TestUtils.add_test` method (or the `ADD_TEST` macro) to add all the test functions you have
 need), and then call `TestUtils.run_tests`:

```c
#include "./api/my_first_api_impl_test.h"
#include "./api/my_second_api_impl_test.h"
#include "./lib/test_utils.h"

int main() {
    test_utils()->add_test("This test does something...", MyFirstApiImpl__my_first_method__test);
    test_utils()->add_test("We do this and expect that...", MyFirstApiImpl__my_second_method__test);
    test_utils()->add_test("MyFirstApiImpl.my_third_method", MyFirstApiImpl__my_third_method__test);

    ADD_TEST(MySecondApiImpl__my_first_method__test);
    ADD_TEST(MySecondApiImpl__my_second_method__test);
    ADD_TEST(MySecondApiImpl__my_third_method__test);

    test_utils()->run_tests();
}
```

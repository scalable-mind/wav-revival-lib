#include <api/uint64_array_api_impl_test.h>
#include <tiny_test/test_utils.h>

int main(int argc, char* argv[]) {
    ADD_TEST(Uint64ArrayApiImpl__push_back__test_1);
    ADD_TEST(Uint64ArrayApiImpl__shrink_to_fit__test_1);
    ADD_TEST(Uint64ArrayApiImpl__push_back__test_2);
    ADD_TEST(Uint64ArrayApiImpl__end__test_1);

    test_utils()->run_tests();
}

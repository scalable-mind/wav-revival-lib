cmake_minimum_required(VERSION 3.10)
project(wav_revival_lib)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

find_package(Catch2 REQUIRED)

#include_directories("C:/Program Files (x86)/Catch2/include")
include_directories(src/main/c)
include_directories(src/test/c)

# targets
set(TARGET_CATCH2_TEST src/test/c/test_catch2.cpp)

# sources
aux_source_directory(src/main/c/api/ API_SOURCES)
aux_source_directory(src/main/c/util/ UTIL_SOURCES)
aux_source_directory(src/test/c/api/ API_TEST_SOURCES)
aux_source_directory(src/test/c/util/ UTIL_TEST_SOURCES)

add_executable(tests_catch2 ${API_SOURCES} ${UTIL_SOURCES} ${API_TEST_SOURCES} ${UTIL_TEST_SOURCES}
        ${TARGET_CATCH2_TEST})

add_library(WavRevival SHARED ${API_SOURCES} ${UTIL_SOURCES})

target_link_libraries(tests_catch2 Catch2::Catch2)

enable_testing()
include(Catch)
catch_discover_tests(tests_catch2)

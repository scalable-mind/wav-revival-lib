cmake_minimum_required(VERSION 3.10)
project(wav_revival_lib C)

include_directories(src/main/c)
include_directories(src/test/c)
include_directories(src/lib/c)

# targets
set(TARGET_TEST src/test/c/test_runner.c)
set(TARGET_LIB src/main/c/export/wav-revival-lib.c)

# sources
aux_source_directory(src/main/c/api/ API_SOURCES)
aux_source_directory(src/main/c/util/ UTIL_SOURCES)
aux_source_directory(src/test/c/api/ API_TEST_SOURCES)
aux_source_directory(src/lib/c/tiny_test/ TINY_TEST_SOURCES)

add_executable(tests ${API_SOURCES} ${UTIL_SOURCES} ${API_TEST_SOURCES} ${TINY_TEST_SOURCES} ${TARGET_TEST})
add_library(wavrevival SHARED ${API_SOURCES} ${UTIL_SOURCES} ${TARGET_LIB})
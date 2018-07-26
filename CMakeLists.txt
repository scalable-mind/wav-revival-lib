cmake_minimum_required(VERSION 3.10)
project(wav_revival_lib C)

include_directories(src/main/c)
include_directories(src/test/c)

# targets
set(TARGET_TEST src/test/c/test_runner.c)

# sources
aux_source_directory(src/main/c/api/ API_SOURCES)
aux_source_directory(src/test/c/api/ API_TEST_SOURCES)
aux_source_directory(src/test/c/lib/ TINY_TEST_SOURCES)

add_executable(tests ${API_SOURCES} ${API_TEST_SOURCES} ${TINY_TEST_SOURCES} ${TARGET_TEST})

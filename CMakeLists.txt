cmake_minimum_required(VERSION 3.10)
project(wav_revival_lib)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set (wav_revival_VERSION_MAJOR 0)
set (wav_revival_VERSION_MINOR 1)
set (wav_revival_VERSION_PATCH 0)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt
        BUILD missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


include_directories(src/main/c)
include_directories(src/test/c)

# targets
set(TARGET_CATCH2_TEST src/test/c/test_catch2.cpp)

# sources
aux_source_directory(src/main/c/api/ API_SOURCES)
aux_source_directory(src/main/c/util/ UTIL_SOURCES)
aux_source_directory(src/test/c/api/ API_TEST_SOURCES)
aux_source_directory(src/test/c/util/ UTIL_TEST_SOURCES)

file(GLOB INSTALL_HEADERS_API "src/main/c/api/*.h")
file(GLOB INSTALL_HEADERS_DOMAIN "src/main/c/domain/*.h")
file(GLOB INSTALL_HEADERS_UTIL "src/main/c/util/*.h")
file(GLOB INSTALL_HEADERS_MACROS "src/main/c/macros/*.h")
file(GLOB INSTALL_HEADERS_GENERAL "src/main/c/*.h")

add_library(wav-revival SHARED ${API_SOURCES} ${UTIL_SOURCES})

add_executable(tests_catch2 ${API_TEST_SOURCES} ${UTIL_TEST_SOURCES}
        ${TARGET_CATCH2_TEST})

target_link_libraries(tests_catch2 Catch2::Catch2)
target_link_libraries(tests_catch2 wav-revival)

enable_testing()
find_package(Catch2 REQUIRED)
message(STATUS ${Catch2_DIR})
include(Catch)
catch_discover_tests(tests_catch2)

install(FILES ${INSTALL_HEADERS_API} DESTINATION include/wav_revival/api)
install(FILES ${INSTALL_HEADERS_DOMAIN} DESTINATION include/wav_revival/domain)
install(FILES ${INSTALL_HEADERS_UTIL} DESTINATION include/wav_revival/util)
install(FILES ${INSTALL_HEADERS_UTIL} DESTINATION include/wav_revival/macros)
install(FILES ${INSTALL_HEADERS_GENERAL} DESTINATION include/wav_revival)
install(TARGETS wav-revival
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
install(TARGETS tests_catch2 DESTINATION bin)

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
        "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${wav_revival_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${wav_revival_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${wav_revival_VERSION_PATCH}")
include (CPack)

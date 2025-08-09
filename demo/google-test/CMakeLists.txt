# Google Test demo
# This is based on https://google.github.io/googletest/quickstart-cmake.html
project(google-test-demo
    VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of Testcontainers C in Google Test")

set(TARGET_OUT ${PROJECT_NAME}.out)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${TARGET_OUT} test.cpp)
add_dependencies(${TARGET_OUT} testcontainers-c-shim)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
target_link_libraries(${TARGET_OUT} PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${TARGET_OUT})

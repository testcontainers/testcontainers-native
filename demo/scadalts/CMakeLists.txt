# Google Test demo
# This is based on https://google.github.io/googletest/quickstart-cmake.html
project(scadalts-demo
    VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of Testcontainers C with SCADA-LTS")

set(TARGET_OUT ${PROJECT_NAME}.out)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
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

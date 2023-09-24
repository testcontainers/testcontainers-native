cmake_minimum_required (VERSION 3.16.3)
project (TESTCONTAINERS-C)

include(CTest)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/testcontainers-c)

add_subdirectory(testcontainers-c)
add_subdirectory(modules)
add_subdirectory(demo)

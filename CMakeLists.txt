cmake_minimum_required (VERSION 3.16.3)
project (TESTCONTAINERS-NATIVE)

include(CTest)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/testcontainers-native)

add_subdirectory(testcontainers-native)
add_subdirectory(modules)
if(NOT DEFINED SKIP_DEMOS)
    add_subdirectory(demo)
endif()

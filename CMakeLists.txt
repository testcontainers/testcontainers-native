cmake_minimum_required (VERSION 3.26)
project (TESTCONTAINERS-C
    VERSION 0.1.0
    DESCRIPTION "Testcontainers for C and other native languages"
    LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CTest)

add_subdirectory(testcontainers-bridge)
add_subdirectory(testcontainers-c)
add_subdirectory(testcontainers-cpp)
add_subdirectory(modules)
if(NOT DEFINED SKIP_DEMOS)
    add_subdirectory(demo)
endif()

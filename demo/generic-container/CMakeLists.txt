cmake_minimum_required (VERSION 3.26)
project (generic-container-demo
    VERSION 0.1.0
    DESCRIPTION "Demonstrates usage of the generic container API in a simple main app"
    LANGUAGES C
)

set(TARGET_OUT ${PROJECT_NAME}.out)

file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Vanilla Demo for WireMock
add_executable(${TARGET_OUT} generic_container_demo.c)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
add_test(NAME generic_container_demo COMMAND ${TARGET_OUT})

cmake_minimum_required (VERSION 3.26)
project (wiremock-demo
    VERSION 0.1.0
    DESCRIPTION "Demonstrates usage of the WireMock module for Testcontainers C in a simple main app"
    LANGUAGES C
)

set(TARGET_OUT demo_wiremock_module.out)

file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# WireMock Module demo
add_executable(${TARGET_OUT} wiremock_module_demo.c)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c-wiremock)

# FIXME: API call crashes on the recent version
# add_test(NAME wiremock_module_demo COMMAND ${TARGET_OUT})

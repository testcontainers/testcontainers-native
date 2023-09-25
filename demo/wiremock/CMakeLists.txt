project(testcontainers-c-wiremock-demo
    VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of the WireMock module for Testcontainers C in a simple main app")

set(TARGET_OUT demo_wiremock_module.out)

include_directories(${testcontainers-c_SOURCE_DIR})
# WORKING_DIRECTORY breaks shared lib loading
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# WireMock Module demo
add_executable(${TARGET_OUT} wiremock_module_demo.c)
add_dependencies(${TARGET_OUT} testcontainers-c-shim)
target_include_directories(${TARGET_OUT} PRIVATE ${testcontainers-c-wiremock_SOURCE_DIR})
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c-wiremock)
add_test(NAME wiremock_module_demo COMMAND ${TARGET_OUT})

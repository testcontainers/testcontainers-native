project(testcontainers-native-wiremock-demo
    VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of the WireMock module for Testcontainers Native in a simple main app")

set(TARGET_OUT demo_wiremock_module.out)

include_directories(${testcontainers-native_SOURCE_DIR})
# WORKING_DIRECTORY breaks shared lib loading
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# WireMock Module demo
add_executable(${TARGET_OUT} wiremock_module_demo.c)
add_dependencies(${TARGET_OUT} testcontainers-native-shim)
target_include_directories(${TARGET_OUT} PRIVATE ${testcontainers-native-wiremock_SOURCE_DIR})
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-native)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-native-wiremock)
add_test(NAME wiremock_module_demo COMMAND ${TARGET_OUT})

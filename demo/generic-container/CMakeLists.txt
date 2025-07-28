project(testcontainers-native-generic-container-demo
    VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of the generic container API in a simple main app")

set(TARGET_OUT demo_generic_container.out)

include_directories(${testcontainers-native_SOURCE_DIR})
# WORKING_DIRECTORY breaks shared lib loading
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Vanilla Demo for WireMock
add_executable(${TARGET_OUT} generic_container_demo.c)
add_dependencies(${TARGET_OUT} testcontainers-native-shim)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-native)
add_test(NAME generic_container_demo COMMAND ${TARGET_OUT})

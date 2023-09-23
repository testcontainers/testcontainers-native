project(testcontainers-c-wiremock-demo VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of the WireMock module for Testcontainers C in a simple main app")

set(TARGET_OUT demo.out)

add_executable(${TARGET_OUT}
    wiremock_demo.c
)

include_directories(${testcontainers-c_SOURCE_DIR})
include_directories(${testcontainers-c-wiremock_SOURCE_DIR})

target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c-wiremock)


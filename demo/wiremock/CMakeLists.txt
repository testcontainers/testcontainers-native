project(testcontainers-c-wiremock-demo VERSION 0.0.1
    DESCRIPTION "Demonstrates usage of the WireMock module for Testcontainers C in a simple main app")

set(TARGET_OUT demo_wiremock_module.out)
set(VANILLA_OUT demo_vanilla.out)

include_directories(${testcontainers-c_SOURCE_DIR})

add_executable(${VANILLA_OUT} wiremock_demo_vanilla.c)
add_dependencies(${VANILLA_OUT} testcontainers-c-shim)
target_link_libraries(${VANILLA_OUT} PRIVATE testcontainers-c)
add_test(NAME vanilla_smoke COMMAND ${VANILLA_OUT})

add_executable(${TARGET_OUT} wiremock_demo.c)
add_dependencies(${VANILLA_OUT} testcontainers-c-shim)
target_include_directories(${TARGET_OUT} PRIVATE ${testcontainers-c-wiremock_SOURCE_DIR})
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c-wiremock)
add_test(NAME wiremock_module_smoke COMMAND ${TARGET_OUT})

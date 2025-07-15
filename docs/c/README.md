# Using Testcontainers for C

You can use the `testcontainers-c` library with common C
unit testing frameworks and, soon, with package managers.

!!! note
    This section is coming soon.
    All contributions are welcome, just submit a pull request!

## Installing the library

For now, you can build and install the library locally.
You can also use CPM in CMake, or another similar tool
that automates pulling and building of CMake projects from GitHub,

### Building locally

You need CMake 3.16.3 and Golang 1.22.5+.
At the moment there are not so many configuration flags, so the build is straightforward:

```shell
cmake .
cmake --build .
ctest --output-on-failure
```

### Installing with CPM

You can also use [CPM](https://github.com/cpm-cmake/CPM.cmake) in CMake:

```cmake
include(cmake/CPM.cmake)
CPMAddPackage(
    NAME testcontainers-c
    VERSION 0.0.2
    GITHUB_REPOSITORY "testcontainers/testcontainers-c"
    OPTIONS "SKIP_DEMOS TRUE")
```

## Using the Library

!!! note
    More frameworks will be documented soon.
    All contributions are welcome, just submit a pull request!

### CMake Tests (CTest)

The first way to use Testcontainers are building native executables with them.
Some frameworks like CTest provide wrappers for such executables.
In such examples, you do not even have to worry about releasing the Testcontainers resources, because
Testcontainers for Go has automatic resource de-provisioning.

#### Initializing the Testcontainers fixture

```c
#include <stdio.h>
#include <string.h>
#include "testcontainers-c-wiremock.h"

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_WIREMOCK_IMAGE);
    int requestId = tc_wm_new_default_container();
    //FIXME: This method is bogus
    tc_wm_with_mapping(requestId, "test_data/hello.json", "hello");
    tc_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello2.json");
    struct tc_run_container_return ret = tc_run_container(requestId);
    int containerId = ret.r0;
    if (!ret.r1) {
        printf("Failed to run the container: %s\n", ret.r2);
        if (containerId != -1) { // Print container log
            char* log = tc_get_container_log(containerId);
            if (log != NULL) {
                printf("\n%s\n", log);
            }
        }
        return -1;
    }
```

#### Testing the Startup

```c
    printf("Getting WireMock stub API mappings to test initialization\n");
    struct WireMock_Mapping mapping = tc_wm_get_mappings(containerId);
    if (mapping.responseCode != 200) {
        printf("Failed to get WireMock mapping: %s\n", mapping.error);
        return -1;
    } else {
        printf("WireMock Mapping:\n%s\n", mapping.json);
    }
```

#### Sending a Test Request

As a first step, you can send a test request to the container.
Then, this code will be replaced by a real test of your application.

```c
    printf("Sending HTTP request to the container\n");
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, "/hello");
    if (response.r0 == -1) {
        printf("Failed to send HTTP request: %s\n", response.r2);
        return -1;
    }
    if (response.r0 != 200) {
        printf("Received wrong response code: %d instead of %d\n%s\n", response.r0, 200, response.r2);
        return -1;
    }
    printf("Server Response: HTTP-%d\n%s\n\n", response.r0, response.r1);
    return 0;
}
```

In CMake, once the `testcontainers-c` is installed (see above),
you can use it in test executables built by CMake:

```cmake
enable_testing()
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${TARGET_OUT} mytest.cpp)
add_dependencies(${TARGET_OUT} testcontainers-c-shim)
target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
add_test(NAME wiremock_module_demo COMMAND ${TARGET_OUT})
```

After building the project, you can run `ctest`:

```shell
cmake --build .
ctest --output-on-failure
```

## Demos

- [Using the generic Testcontainers C API](../../demo/generic-container/README.md)
- [Using the WireMock module](../../demo/wiremock/README.md)

# Using Testcontainers in C++

At the moment, there is no dedicated C++ binding library/header,
but it is on [our roadmap](../../ROADMAP.md).
You can use the `testcontainers-c` library directly
in all C++ testing frameworks.

## Google Test

Like in other unit test frameworks,
in [GoogleTest](https://google.github.io/googletest/) you can deploy a Testcontainers as a test fixture,
with proper setup and tear-down methods.

```cpp
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "testcontainers-c.h"

class WireMockTestContainer : public ::testing::Test {

const char* WIREMOCK_IMAGE = "wiremock/wiremock:3.0.1-1";
const char* WIREMOCK_ADMIN_MAPPING_ENDPOINT = "/__admin/mappings";

protected:
    void SetUp() override {
        std::cout << "Creating new container: " << WIREMOCK_IMAGE << '\n';
        int requestId = tc_new_container_request(WIREMOCK_IMAGE);
        tc_with_exposed_tcp_port(requestId, 8080);
        tc_with_wait_for_http(requestId, 8080, WIREMOCK_ADMIN_MAPPING_ENDPOINT);
        tc_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello.json");

        struct tc_run_container_return ret = tc_run_container(requestId);
        containerId = ret.r0;

        EXPECT_TRUE(ret.r1) << "Failed to run the container: " << ret.r2;
    };

    void TearDown() override {
        char* error = tc_terminate_container(containerId);
        ASSERT_EQ(error, nullptr) << "Failed to terminate the container after the test: " << error;
    };

    int containerId;
};
```

Then, you can define new tests by referring to the container via `containerId`.

```cpp
TEST_F(WireMockTestContainer, HelloWorld) {
    std::cout << "Sending HTTP request to the container\n";
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, "/hello");

    ASSERT_NE(response.r0, -1) << "Failed to send HTTP request: " << response.r2;
    ASSERT_EQ(response.r0, 200) << "Received wrong response code: " << response.r1 << response.r2;

    std::cout << "Server Response: HTTP-" << response.r0 << '\n' << response.r1 << '\n';
}
```

## Demos

- [Using Testcontainers C in Google Test (C++)](../../demo/google-test/README.md)

#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "testcontainers.hpp"

using namespace testcontainers;

class WireMockTestContainerClass : public ::testing::Test {
    const char* WIREMOCK_IMAGE                  = "wiremock/wiremock:3.0.1-1";
    const char* WIREMOCK_ADMIN_MAPPING_ENDPOINT = "/__admin/mappings";

protected:
    void SetUp() override {
        using namespace std::literals;

        builder.expose_port(TcpPort{8080}).wait_for_http(TcpPort{8080}, WIREMOCK_ADMIN_MAPPING_ENDPOINT);
    }

    Container::Builder builder = Container::Builder{WIREMOCK_IMAGE};
};

/// This test runs a "Hello World" example.
TEST_F(WireMockTestContainerClass, HelloWorld) {
    auto container = builder.with_file("test_data/hello.json", "/home/wiremock/mappings/hello.json").build();
    ASSERT_TRUE(container.has_value()) << "Failed to run the container";

    auto [code, response] = container->send_http(HttpMethod::Get, TcpPort{8080}, "/hello");
    ASSERT_EQ(code, 200) << "Expected 200 OK. Received response: " << response;
}

/// This test responds to an HTTP request with response from a
/// prepared file.
TEST_F(WireMockTestContainerClass, HelloWorldFromResource) {
    auto container = builder.with_file("test_data/hello_with_resource.json", "/home/wiremock/mappings/hello2.json")
                         .with_file("test_data/response.xml", "/home/wiremock/__files/response.xml")
                         .build();
    ASSERT_TRUE(container.has_value()) << "Failed to run the container";

    auto [code, response] = container->send_http(HttpMethod::Get, TcpPort{8080}, "/hello-from-resource");
    ASSERT_EQ(code, 200) << "Expected 200 OK. Received response: " << response;
}

/// This test performs a request that is guaranteed to fail and
/// return an HTTP-500.
TEST_F(WireMockTestContainerClass, HelloWorldFromMissingResource) {
    auto container =
        builder.with_file("test_data/hello_with_missing_resource.json", "/home/wiremock/mappings/hello3.json").build();
    ASSERT_TRUE(container.has_value()) << "Failed to run the container";

    auto [code, response] = container->send_http(HttpMethod::Get, TcpPort{8080}, "/hello-from-missing-resource");
    ASSERT_EQ(code, 500) << "Expected 500 Internal Server Error. Received response: " << response;
}

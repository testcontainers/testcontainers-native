#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "testcontainers.hpp"

using namespace testcontainers;

class WireMockTestContainerClass : public ::testing::Test {

const char* WIREMOCK_IMAGE = "wiremock/wiremock:3.0.1-1";
const char* WIREMOCK_ADMIN_MAPPING_ENDPOINT = "/__admin/mappings";

protected:
    void SetUp() override {
        std::cout << "Creating new container: " << WIREMOCK_IMAGE << '\n';

        container = std::make_unique<Container>(WIREMOCK_IMAGE);
        container->expose_port(TcpPort{8080});
        container->wait_for_http(TcpPort{8080}, WIREMOCK_ADMIN_MAPPING_ENDPOINT);
        container->with_file("test_data/hello.json", "/home/wiremock/mappings/hello.json");
        container->with_file("test_data/hello_with_resource.json", "/home/wiremock/mappings/hello2.json");
        container->with_file("test_data/hello_with_missing_resource.json", "/home/wiremock/mappings/hello3.json");
        container->with_file("test_data/response.xml", "/home/wiremock/__files/response.xml");

        auto result = container->run();

        EXPECT_TRUE(!result.has_value()) << "Failed to run the container: " << result.error();
    };

    std::unique_ptr<Container> container;
};

TEST_F(WireMockTestContainerClass, HelloWorld) {
    std::cout << "Sending HTTP request to the container\n";

    auto [code, response] = container->send_http(HttpMethod::Get, TcpPort{8080}, "/hello");

    ASSERT_TRUE(response.has_value()) << "Failed to send HTTP request: " << response.error();
    ASSERT_EQ(code, 200) << "Received wrong response code: " << response.error();

    std::cout << "Server Response: HTTP-" << code << '\n' << response.value() << '\n';
}

TEST_F(WireMockTestContainerClass, HelloWorldFromResource) {
    std::cout << "Sending HTTP request to the container\n";

    auto [code, response] = container->send_http(HttpMethod::Get, TcpPort{8080}, "/hello-from-resource");

    ASSERT_TRUE(response.has_value()) << "Failed to send HTTP request: " << response.error();
    ASSERT_EQ(code, 200) << "Received wrong response code: " << response.error();

    std::cout << "Server Response: HTTP-" << code << '\n' << response.value() << '\n';
}

TEST_F(WireMockTestContainerClass, HelloWorldFromMissingResource) {
    std::cout << "Sending HTTP request to the container\n";

    auto [code, response] = container->send_http(HttpMethod::Get, TcpPort{8080}, "/hello-from-missing-resource");

    ASSERT_EQ(code, 500) << "The request should have failed";
}

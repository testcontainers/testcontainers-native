#include <iostream>
#include <string>
#include <gtest/gtest.h>

extern "C" {
#include "testcontainers-c/container.h"
}

class WireMockTestContainer : public ::testing::Test {

const char* WIREMOCK_IMAGE = "wiremock/wiremock:3.0.1-1";
const char* WIREMOCK_ADMIN_MAPPING_ENDPOINT = "/__admin/mappings";

protected:
    void SetUp() override {
        std::cout << "Creating new container: " << WIREMOCK_IMAGE << '\n';

        int requestId = tc_container_create(WIREMOCK_IMAGE);
        tc_container_with_exposed_tcp_port(requestId, 8080);
        tc_container_with_wait_for_http(requestId, 8080, WIREMOCK_ADMIN_MAPPING_ENDPOINT);
        tc_container_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello.json");
        tc_container_with_file(requestId, "test_data/hello_with_resource.json", "/home/wiremock/mappings/hello2.json");
        tc_container_with_file(requestId, "test_data/hello_with_missing_resource.json", "/home/wiremock/mappings/hello3.json");
        tc_container_with_file(requestId, "test_data/response.xml", "/home/wiremock/__files/response.xml");

        char* error;
        int containerId = tc_container_run(requestId, error);

        EXPECT_TRUE(containerId != -1) << "Failed to run the container: " << error;
    };

    void TearDown() override {
        char* error = tc_container_terminate(containerId);
        ASSERT_EQ(error, nullptr) << "Failed to terminate the container after the test: " << error;
    };

    int containerId;
};

TEST_F(WireMockTestContainer, HelloWorld) {
    std::cout << "Sending HTTP request to the container\n";
    char *response_body;
    char *error;
    int response_code = tc_container_send_http_get(containerId, 8080, "/hello", response_body, error);

    ASSERT_NE(response_code, -1) << "Failed to send HTTP request: " << error;
    ASSERT_EQ(response_code, 200) << "Received wrong response code: " << response_body << error;

    std::cout << "Server Response: HTTP-" << response_code << '\n' << response_body << '\n';
}

TEST_F(WireMockTestContainer, HelloWorldFromResource) {
    std::cout << "Sending HTTP request to the container\n";
    char *response_body;
    char *error;
    int response_code = tc_container_send_http_get(containerId, 8080, "/hello-from-resource", response_body, error);

    ASSERT_NE(response_code, -1) << "Failed to send HTTP request: " << error;
    ASSERT_EQ(response_code, 200) << "Received wrong response code: " << response_body << error;

    std::cout << "Server Response: HTTP-" << response_code << '\n' << response_body << '\n';
}

TEST_F(WireMockTestContainer, HelloWorldFromMissingResource) {
    std::cout << "Sending HTTP request to the container\n";
    char *response_body;
    char *error;
    int response_code = tc_container_send_http_get(containerId, 8080, "/hello-from-missing-resource", response_body, error);

    ASSERT_EQ(response_code, 500) << "The request should have failed";
}

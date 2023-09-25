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
        tc_with_file(requestId, "test_data/hello_with_resource.json", "/home/wiremock/mappings/hello2.json");
        tc_with_file(requestId, "test_data/hello_with_missing_resource.json", "/home/wiremock/mappings/hello3.json");
        tc_with_file(requestId, "test_data/response.xml", "/home/wiremock/__files/response.xml");
        
        struct tc_run_container_return ret = tc_run_container(requestId);
        containerId = ret.r0;
        
        EXPECT_TRUE(ret.r1) << "Failed to run the container: " << ret.r2;
    };

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    };

    int containerId;
};

TEST_F(WireMockTestContainer, HelloWorld) {
    std::cout << "Sending HTTP request to the container\n";
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, "/hello");
    
    ASSERT_NE(response.r0, -1) << "Failed to send HTTP request: " << response.r2;
    ASSERT_EQ(response.r0, 200) << "Received wrong response code: " << response.r1 << response.r2;
 
    std::cout << "Server Response: HTTP-" << response.r0 << '\n' << response.r1 << '\n';
}

TEST_F(WireMockTestContainer, HelloWorldFromResource) {
    std::cout << "Sending HTTP request to the container\n";
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, "/hello-from-resource");
    
    ASSERT_NE(response.r0, -1) << "Failed to send HTTP request: " << response.r2;
    ASSERT_EQ(response.r0, 200) << "Received wrong response code: " << response.r1 << response.r2;
 
    std::cout << "Server Response: HTTP-" << response.r0 << '\n' << response.r1 << '\n';
}

TEST_F(WireMockTestContainer, HelloWorldFromMissingResource) {
    std::cout << "Sending HTTP request to the container\n";
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, "/hello-from-missing-resource");
    
    ASSERT_EQ(response.r0, 500) << "The request should have failed";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

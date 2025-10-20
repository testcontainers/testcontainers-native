#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "testcontainers-c.h"

class WireMockTestContainer : public ::testing::Test {

const char* SCADALTS_IMAGE = "dudanov/openscada:latest";
const char* SCADALTS_COMPOSE = "./docker-compose.yml";
const char* SCADALTS_ENDPOINT = "/";

protected:
    void SetUp() override {
        std::cout << "Creating new container: " << SCADALTS_IMAGE << '\n';
        
        struct tc_new_docker_composer_return = tc_new_docker_compose(SCADALTS_COMPOSE);
        compose_id = ret.r0;
        
        EXPECT_TRUE(ret.r1) << "Failed to run the container: " << ret.r2;
    };

    void TearDown() override {
        char* error = tc_terminate_compose(compose_id);
        ASSERT_EQ(error, nullptr) << "Failed to terminate the container after the test: " << error;
    };

    int compose_id;
};

TEST_F(WireMockTestContainer, HelloWorld) {
    std::cout << "Sending HTTP request to the container\n";
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, "/");
    
    ASSERT_NE(response.r0, -1) << "Failed to send HTTP request: " << response.r2;
    ASSERT_EQ(response.r0, 200) << "Received wrong response code: " << response.r1 << response.r2;
 
    std::cout << "Server Response: HTTP-" << response.r0 << '\n' << response.r1 << '\n';
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

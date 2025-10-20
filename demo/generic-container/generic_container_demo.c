#include <stdio.h>
#include "testcontainers-c/container.h"

#define DEFAULT_IMAGE "wiremock/wiremock:3.0.1-1"

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_IMAGE);
    int requestId = tc_container_create(DEFAULT_IMAGE);
    tc_container_with_exposed_tcp_port(requestId, 8080);
    tc_container_with_wait_for_http(requestId, 8080, "/__admin/mappings");
    tc_container_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello.json");
    char* error;
    int containerId = tc_container_run(requestId, error);
    if (containerId == -1) {
        printf("Failed to run the container: %s\n", error);
        return -1;
    }

    printf("Sending HTTP request to the container\n");
    char *response_body;
    int response_code = tc_container_send_http_get(containerId, 8080, "/hello", response_body, error);
    if (response_code == -1) {
        printf("Failed to send HTTP request: %s\n", error);
        return -1;
    }
    if (response_code != 200) {
        printf("Received wrong response code: %d instead of %d\n%s\n%s\n", response_code, 200, response_body, error);
        return -1;
    }
    printf("Server Response: HTTP-%d\n%s\n\n", response_code, response_body);
    return 0;
}


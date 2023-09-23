#include <stdio.h>
#include <string.h>
#include "testcontainers-c.h"

#define DEFAULT_IMAGE "wiremock/wiremock:3.0.1-1"
#define GOSTRING(X) (GoString) {X, strlen(X)}

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_IMAGE);
    int requestId = tc_new_container_request(DEFAULT_IMAGE);
    tc_with_exposed_tcp_port(requestId, 8080);
    tc_with_wait_for_http(requestId, 8080, GOSTRING("/__admin/mappings"));
    tc_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello.json");
    struct tc_run_container_return ret = tc_run_container(requestId);
    int containerId = ret.r0;
    if (containerId == -1) {
        printf("Failed to run the container: %s\n", ret.r1);
        return -1;
    }

    printf("Sending HTTP request to the container\n");
    struct tc_send_http_get_return response = tc_send_http_get(containerId, 8080, GOSTRING("/hello"));
    if (response.r0 == -1) {
        printf("Failed to send HTTP request: %s\n", response.r2);
        return -1;
    }
    if (response.r0 != 200) {
        printf("Received wrong response code: %d instead of %d\n%s\n%s\n", response.r0, 200, response.r1, response.r2);
        return -1;
    }
    printf("Server Response: HTTP-%d\n%s\n\n", response.r0, response.r1);
    return 0;
}


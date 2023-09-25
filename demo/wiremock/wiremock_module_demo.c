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

    printf("Getting WireMock stub API mappings to test initialization\n");
    struct WireMock_Mapping mapping = tc_wm_get_mappings(containerId);
    if (mapping.responseCode != 200) {
        printf("Failed to get WireMock mapping: %s\n", mapping.error);
        return -1;
    } else {
        printf("WireMock Mapping:\n%s\n", mapping.json);
    }

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


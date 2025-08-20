#include <stdio.h>
#include <string.h>
#include "testcontainers-c-wiremock.h"
#include "testcontainers-c/container.h"

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_WIREMOCK_IMAGE);
    int requestId = tc_wm_new_default_container();
    //FIXME: This method is bogus
    // tc_wm_with_mapping(requestId, "test_data/hello.json", "hello");
    tc_container_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello2.json");
    char* error;
    int containerId = tc_container_run(requestId, error);
    if (containerId == -1) {
        printf("Failed to run the container: %s\n", error);
        if (containerId != -1) { // Print container log
            char* log = tc_container_get_log(containerId);
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
    char *response_body;
    int response_code = tc_container_send_http_get(containerId, 8080, "/hello", response_body, error);
    if (response_code == -1) {
        printf("Failed to send HTTP request: %s\n", error);
        return -1;
    }
    if (response_code != 200) {
        printf("Received wrong response code: %d instead of %d\n%s\n", response_code, 200, error);
        return -1;
    }
    printf("Server Response: HTTP-%d\n%s\n\n", response_code, response_body);
    return 0;
}


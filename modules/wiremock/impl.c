#include "testcontainers-c-wiremock.h"
#include "testcontainers-c/container.h"
#include <string.h>
#include <stdio.h>

int tc_wm_new_default_container() {
    return tc_wm_new_container(DEFAULT_WIREMOCK_IMAGE);
}

int tc_wm_new_container(char* image) {
    int requestId = tc_container_create(image);
    tc_container_with_exposed_tcp_port(requestId, 8080);
    tc_container_with_wait_for_http(requestId, 8080, "/__admin/mappings");
    return requestId;
};

void tc_wm_with_mapping(int requestID, char* filePath, char* destination) {
    char dest_file[128] = "";
    strcat(dest_file, WIREMOCK_MAPPINGS_DIR);
    strcat(dest_file, destination);

    // Append extension if missing
    if(strlen(destination) > 5 && !strcmp(destination + strlen(destination) - 5, ".json")) {
        strcat(dest_file, ".json");
    }
    if (strlen(destination) <= 5) {
        strcat(dest_file, ".json");
    }

    printf("DEBUG: %s to %s.\n", filePath, dest_file);
    tc_container_with_file(requestID, filePath, dest_file);
};

struct WireMock_Mapping tc_wm_get_mappings(int containerId) {
    char *response_body;
    char *error;
    int response_code = tc_container_send_http_get(containerId, 8080, "/__admin/mappings", response_body, error);
    if (response_code == -1) {
        char errorMsg[8000] = "";
        sprintf(errorMsg, "Failed to send HTTP request: %s\n", error);
        return (struct WireMock_Mapping) { -1, NULL, errorMsg};
    }
    if (response_code != 200) {
        char errorMsg[8000] = "";
        sprintf(errorMsg, "Received wrong response code: %d instead of %d\n%s\n%s\n", response_code, 200, response_body, error);
        return (struct WireMock_Mapping) { response_code, NULL, errorMsg};
    }

    return (struct WireMock_Mapping) {response_code, response_body, NULL};
};

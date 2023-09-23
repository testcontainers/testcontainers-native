#include "testcontainers-c-wiremock.h"
#include <string.h>
#include <stdio.h>

GoInt tc_wm_new_default_container() {
    return tc_wm_new_container(DEFAULT_WIREMOCK_IMAGE);
}

GoInt tc_wm_new_container(char* image) {
    GoInt requestId = tc_new_container_request(image);
    tc_with_exposed_tcp_port(requestId, 8080);
    tc_with_wait_for_http(requestId, 8080, WM_GOSTRING("/__admin/mappings"));
    return requestId;
};

void tc_wm_with_mapping(GoInt requestID, char* filePath, char* destination) {
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

    printf("DEBUG: %s to %s\n", filePath, dest_file);
    tc_with_file(requestID, filePath, dest_file); 
};

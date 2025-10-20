#include <testcontainers-bridge.h>

#include "testcontainers-c/container.h"

int tc_container_create(const char* image) {
    if (image == NULL) {
        return -1;
    }

    return tc_bridge_new_container_request(image);
}

int tc_container_run(int container_id, char* error) {
    struct tc_bridge_run_container_return result = tc_bridge_run_container(container_id);

    if (error != NULL) {
        error = result.r2;
    }

    if (!result.r1) {
        return -1;
    }

    return result.r0;
}

char* tc_container_terminate(int container_id) { return tc_bridge_terminate_container(container_id); }

char* tc_container_get_log(int container_id) { return tc_bridge_get_container_log(container_id); }

char* tc_container_get_uri(int container_id, int port, char* error) {
    struct tc_bridge_get_uri_return result = tc_bridge_get_uri(container_id, port);

    if (error != NULL) {
        error = result.r2;
    }

    if (result.r1) {
        return result.r0;
    }

    return NULL;
}

void tc_container_with_wait_for_http(int request_id, int port, const char* url) {
    tc_bridge_with_wait_for_http(request_id, port, url);
}

void tc_container_with_file(int request_id, const char* file_path, const char* target_path) {
    tc_bridge_with_file(request_id, file_path, target_path);
}

void tc_container_with_exposed_tcp_port(int request_id, int port) { tc_bridge_with_exposed_tcp_port(request_id, port); }

int tc_container_send_http_get(int container_id, int port, const char* endpoint, char* response_body, char* error) {
    struct tc_bridge_send_http_get_return result = tc_bridge_send_http_get(container_id, port, endpoint);

    if (error != NULL) {
        error = result.r2;
    }

    if (response_body != NULL) {
        response_body = result.r1;
    }

    return result.r0;
}

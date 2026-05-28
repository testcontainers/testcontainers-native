#include <testcontainers-bridge.h>

#include "testcontainers-c/container.h"

#include <assert.h>

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

int tc_container_get_mapped_port(int container_id, int container_port, char** error) {
    struct tc_bridge_get_mapped_port_return result = tc_bridge_get_mapped_port(container_id, container_port);

    int result_port = result.r0;
    bool result_ok = result.r1;
    char* result_error = result.r2;

    if (!result_ok) {
        assert(result_error != NULL);

        if (error != NULL) {
            *error = result_error;
        }

        return -1;
    }

    return result_port;
}


char* tc_container_get_hostname(int container_id, char** error) {
    struct tc_bridge_get_hostname_return result = tc_bridge_get_hostname(container_id);

    char* result_hostname = result.r0;
    bool result_ok = result.r1;
    char* result_error = result.r2;

    if (!result_ok) {
        assert(result_error != NULL);

        if (error != NULL) {
            *error = result_error;
        }

        return NULL;
    }

    return result_hostname;
}

void tc_container_with_wait_for_http(int request_id, int port, const char* url) {
    tc_bridge_with_wait_for_http(request_id, port, url);
}

void tc_container_with_file(int request_id, const char* file_path, const char* target_path) {
    tc_bridge_with_file(request_id, file_path, target_path);
}

void tc_container_with_exposed_tcp_port(int request_id, int port) { tc_bridge_with_exposed_tcp_port(request_id, port); }

int tc_container_with_wait_for_exec(int request_id, const char** cmd, size_t cmd_len) {
    return tc_bridge_with_wait_for_exec(request_id, cmd, cmd_len);
}

void tc_exec_with_exit_code_matcher(int strategy_id, bool (*exit_code_matcher)(int)) {
    tc_bridge_exec_with_exit_code_matcher(strategy_id, exit_code_matcher);
}

void tc_container_with_env(int request_id, const char* name, const char* value) {
    tc_bridge_with_env(request_id, name, value);
}

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

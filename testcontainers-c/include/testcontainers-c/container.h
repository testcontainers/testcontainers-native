#ifndef CONTAINER_H
#define CONTAINER_H

/**
 * @param The container image name.
 * @return -1 if no image is provided or creation fails. Otherwise,
 * the ID of the created container.
 */
int tc_container_create(const char* image);

/**
 *
 */
int tc_container_run(int container_id, char* error);

/**
 *
 */
char* tc_container_terminate(int container_id);

/**
 *
 */
char* tc_container_get_log(int container_id);

/**
 * Retrieves the URI for container with id `container_id`
 * It optionally stores an error message in `error`.
 */
char* tc_container_get_uri(int container_id, int port, char* error);

/**
 *
 */
void tc_container_with_wait_for_http(int request_id, int port, const char* url);

/**
 *
 */
void tc_container_with_file(int request_id, const char* file_path, const char* target_path);

/**
 *
 */
void tc_container_with_exposed_tcp_port(int request_id, int port);

/**
 *  Sends an HTTP GET request to an endpoint in a container.
 *
 * @param container_id The ID of the container.
 * @param port The port of the container HTTP server.
 * @param endpoint The endpoint to request.
 * @param response_body The body of the response.
 * @param error The error message if the request fails.
 * @return Response code from the HTTP request.
 */
int tc_container_send_http_get(int container_id, int port, const char* endpoint, char* response_body, char* error);

#endif // !CONTAINER_H

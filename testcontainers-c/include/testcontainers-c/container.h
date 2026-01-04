#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdbool.h>
#include <stddef.h>

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
 * Retrieves the mapped/exposed port corresponding to the given container port.
 * @param container_id The running container's ID.
 * @param container_port The internal container port number.
 * @param error Pointer to a memory location that will be replaced with a copy of the error message, if an error occurs. 
 *              This argument is optional and NULL can be passed if capturing the error message is not required.
 *              If it is assigned to, the caller assumes ownership of the memory and should call free() on it when no longer needed.
 * @return The mapped port number, or -1 if there was an error.
 */
int tc_container_get_mapped_port(int container_id, int container_port, char** error);

/** 
 * Retrieves the container's hostname.
 * @param container_id The running container's ID.
 * @param error Pointer to a memory location that will be replaced with a copy of the error message, if an error occurs. 
 *              This argument is optional and NULL can be passed if capturing the error message is not required.
 *              If it is assigned to, the caller assumes ownership of the memory and should call free() on it when no longer needed.
 * @return The hostname, or NULL if there was an error.
 */
char* tc_container_get_hostname(int container_id, char** error);

/**
 *
 */
void tc_container_with_wait_for_http(int request_id, int port, const char* url);

/**
 * Configure the container to wait for a specified command to execute successfully. 
 * Once configured, the intended exit code should be specified with tc_exec_with_exit_code_matcher.
 *
 * @param request_id The container request ID.
 * @param cmd Pointer to the start of an array of strings, containing the command and its arguments.
 * @param cmd_len Length of the passed array.
 *
 * @return An 'exec strategy' ID, which can be used to customise the strategy.
 *
 * @see tc_exec_with_exit_code_matcher
 */
int tc_container_with_wait_for_exec(int request_id, const char** cmd, size_t cmd_len);

/**
 * Configure how the exec wait strategy, as previously configured with tc_container_with_wait_for_exec, should
 * interpret the exit code of the command it is executing.
 * 
 * @param strategy_id The strategy ID returned from tc_container_with_wait_for_exec.
 * @param exit_code_matcher Function that takes in the exit code of the command. If it returns true, the container
 * will be considered to be ready.
 * @see tc_exec_with_exit_code_matcher
 */
void tc_exec_with_exit_code_matcher(int strategy_id, bool (*exit_code_matcher)(int));

/**
 * Apply an environment variable with the given name and value to the container.
 * @param request_id The container request ID.
 * @param name The name of the environment variable.
 * @param value The value of the environment variable.
 */
void tc_container_with_env(int request_id, const char* name, const char* value);

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

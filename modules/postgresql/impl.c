#include "testcontainers-c-postgresql.h"
#include "testcontainers-c/container.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_POSTGRESQL_IMAGE_NAME "postgres"
#define DEFAULT_POSTGRESQL_VERSION "18.1"
#define DEFAULT_POSTGRESQL_PORT 5432
#define DEFAULT_POSTGRESQL_IMAGE DEFAULT_POSTGRESQL_IMAGE_NAME ":" DEFAULT_POSTGRESQL_VERSION

#define DEFAULT_POSTGRESQL_USERNAME "postgres"
#define DEFAULT_POSTGRESQL_PASSWORD "postgres"
#define DEFAULT_POSTGRESQL_DB "postgres"

static bool pg_isready_is_success(int exitCode) {
    return exitCode == 0;
}

static const char* pgIsReadyCmd[] = { "pg_isready", "--host", "localhost", "--username", DEFAULT_POSTGRESQL_USERNAME, "--dbname", DEFAULT_POSTGRESQL_DB };
static const size_t pgIsReadyCmdLen = sizeof(pgIsReadyCmd) / sizeof(char*);

int tc_psql_new_default_container() {
    return tc_psql_new_container(DEFAULT_POSTGRESQL_IMAGE);
}

int tc_psql_new_container(const char *image) {
    int requestId = tc_container_create(image);

    tc_container_with_env(requestId, "POSTGRES_USER", DEFAULT_POSTGRESQL_USERNAME);
    tc_container_with_env(requestId, "POSTGRES_PASSWORD", DEFAULT_POSTGRESQL_PASSWORD);
    tc_container_with_env(requestId, "POSTGRES_DB", DEFAULT_POSTGRESQL_DB);
    tc_container_with_exposed_tcp_port(requestId, DEFAULT_POSTGRESQL_PORT);

    int strategyId = tc_container_with_wait_for_exec(requestId, pgIsReadyCmd, pgIsReadyCmdLen);
    tc_exec_with_exit_code_matcher(strategyId, pg_isready_is_success);

    // TODO: in other Testcontainers implementations, they pass some command parameters to the container
    // to disable certain hardening features and improve performance as the container is ephemeral. We
    // should consider doing the same if we add the ability to modify container commands to the bridge.
    // See: https://www.postgresql.org/docs/current/non-durability.html

    return requestId;
}

int tc_psql_get_connection_string(int containerId, char* buffer, size_t bufferLen) {
    if (buffer == NULL || bufferLen <= 0) {
        // Invalid arguments
        return -1;
    }

    char* error;

    const char* hostname = tc_container_get_hostname(containerId, &error);
    if (hostname == NULL) {
        fprintf(stderr, "Failed to get PostgreSQL hostname: %s", error);
        free(error);
        return -1;
    }

    int port = tc_container_get_mapped_port(containerId, DEFAULT_POSTGRESQL_PORT, &error);
    if (port == -1) {
        fprintf(stderr, "Failed to get PostgreSQL mapped port: %s", error);
        free(error);
        return -1;
    }


    int written = snprintf(
        buffer, 
        bufferLen, 
        "host=%s port=%d user=%s password=%s dbname=%s",
        hostname,
        port,
        DEFAULT_POSTGRESQL_USERNAME,
        DEFAULT_POSTGRESQL_PASSWORD,
        DEFAULT_POSTGRESQL_DB
    );

    if (written < 0) {
        return -1;
    }

    if (written >= bufferLen) {
        // Provided buffer was not long enough; truncation has occurred
        // If written == bufferLen, that means we missed 1 character because of the null terminator
        return -1;
    }

    return written;
}
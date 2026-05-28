#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <testcontainers-c-postgresql.h>
#include <testcontainers-c/container.h>

#include <libpq-fe.h>

static const char* postgresqlImage = "postgres:18.1";

int main() {
    printf("Creating new PostgreSQL container. Using image: %s\n", postgresqlImage);
    int requestId = tc_psql_new_container(postgresqlImage);

    char* error;
    int containerId = tc_container_run(requestId, error);
    
    if (containerId == -1) {
        fprintf(stderr, "Failed to run the container: %s\n", error);
        return EXIT_FAILURE;
    }

    char connectionString[512];
    int bytesWritten = tc_psql_get_connection_string(containerId, connectionString, sizeof(connectionString));
    if (bytesWritten == -1) {
        fprintf(stderr, "Failed to get connection string\n");
        return EXIT_FAILURE;
    }

    printf("Connecting to PostgreSQL using connection string: %s\n", connectionString);

    PGconn* conn = PQconnectdb(connectionString);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Failed to connect to PostgreSQL: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    printf("Successfully connected to PostgreSQL!\n");
    PQfinish(conn);

    error = tc_container_terminate(containerId);
    if (error != NULL) {
        fprintf(stderr, "Failed to terminate PostgreSQL container: %s", error);
        return EXIT_FAILURE;
    }

    printf("Terminated PostgreSQL container\n");

    return EXIT_SUCCESS;
}


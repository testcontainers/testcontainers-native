#ifndef TESTCONTAINERS_POSTGRESQL_H
#define TESTCONTAINERS_POSTGRESQL_H

#include <stddef.h>

/// @brief Creates a container request with a default image, exposed port and init logic
/// @return Container request ID
int tc_psql_new_default_container();

/// @brief Creates a container request with a specified image, exposed port and init logic
/// @param image Full image name
/// @return Container request ID
int tc_psql_new_container(const char* image);

/// @brief Gets the connection string that can be used with libpq/libpqxx to connect to the database.
/// @param containerId Container ID
/// @param buffer A buffer to write the connection string into. The written value will be null-terminated.
/// @param bufferLen The length of the buffer.
/// @return The number of bytes written, excluding the null terminator, or -1 if there was an error.
int tc_psql_get_connection_string(int containerId, char* buffer, size_t bufferLen);

// TODO: further container customisation to allow configuring database name, username, password, etc.

#endif

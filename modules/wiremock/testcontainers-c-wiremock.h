#include "testcontainers-c.h"

#ifndef TESTCONTAINERS_WIREMOCK_H
#define TESTCONTAINERS_WIREMOCK_H

#define DEFAULT_WIREMOCK_IMAGE_NAME "wiremock/wiremock"
#define DEFAULT_WIREMOCK_VERSION "3.1.0-1"
#define DEFAULT_WIREMOCK_PORT "8080"
#define DEFAULT_WIREMOCK_IMAGE DEFAULT_WIREMOCK_IMAGE_NAME ":" DEFAULT_WIREMOCK_VERSION

#define WIREMOCK_MAPPINGS_DIR "/home/wiremock/mappings/"
#define WIREMOCK_FILES_DIR "/home/wiremock/__files/"

#define WM_GOSTRING(X) (GoString) {X, strlen(X)}

/// @brief Creates a container request with a default image, exposed port and init logic
/// @return Container request ID
GoInt tc_wm_new_default_container();

/// @brief Creates a container request with a default image, exposed port and init logic
/// @param image Full image name
/// @return Container request ID
GoInt tc_wm_new_container(char* image);

/// @brief Adds WireMock mapping to the request
/// @param requestID Container Request ID
/// @param filePath Source file in the local filesystem
/// @param destination Destination file, relative to the mappings dir. Extension is optional
void tc_wm_with_mapping(GoInt requestID, char* filePath, char* destination);

#endif

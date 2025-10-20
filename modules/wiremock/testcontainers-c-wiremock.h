#ifndef TESTCONTAINERS_WIREMOCK_H
#define TESTCONTAINERS_WIREMOCK_H

#define DEFAULT_WIREMOCK_IMAGE_NAME "wiremock/wiremock"
#define DEFAULT_WIREMOCK_VERSION "3.1.0-1"
#define DEFAULT_WIREMOCK_PORT "8080"
#define DEFAULT_WIREMOCK_IMAGE DEFAULT_WIREMOCK_IMAGE_NAME ":" DEFAULT_WIREMOCK_VERSION

#define WIREMOCK_MAPPINGS_DIR "/home/wiremock/mappings/"
#define WIREMOCK_FILES_DIR "/home/wiremock/__files/"

struct WireMock_Mapping
{
    int responseCode;
    char* json;
    char* error;
};

/// @brief Creates a container request with a default image, exposed port and init logic
/// @return Container request ID
int tc_wm_new_default_container();

/// @brief Creates a container request with a default image, exposed port and init logic
/// @param image Full image name
/// @return Container request ID
int tc_wm_new_container(char* image);

/// @brief Adds WireMock mapping to the request
/// @param requestID Container Request ID
/// @param filePath Source file in the local filesystem
/// @param destination Destination file, relative to the mappings dir. Extension is optional
void tc_wm_with_mapping(int requestID, char* filePath, char* destination);

/// @brief Gets WireMock mappings using Admin API
/// @param containerId Container ID
/// @return Mapping information if response code is 200, error details otherwise
struct WireMock_Mapping tc_wm_get_mappings(int containerId);

#endif

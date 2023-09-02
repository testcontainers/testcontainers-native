#include <stdio.h>
#include <string.h>
#include "testcontainers.h"

#define DEFAULT_IMAGE "wiremock/wiremock:3.0.1-1"
#define GOSTRING(X) (GoString) {X, strlen(X)}

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_IMAGE);
    int requestId = NewContainerRequest(GOSTRING(DEFAULT_IMAGE));
    WithExposedTcpPort(requestId, 8080);
    WithWaitForHttp(requestId, 8080, GOSTRING("/__admin/mappings"));
    WithFile(requestId, GOSTRING("test_data/hello.json"), GOSTRING("/home/wiremock/__files/hello.json"));
    // TODO: pass arrays in a fancy way: int customizers[1] = {customizerId}; 
    struct RunContainer_return ret = RunContainer(requestId);
    int containerId = ret.r0;
    if (containerId == -1) {
        printf("Failed to run the container: %s\n", ret.r1);
        return -1;
    }

    printf("Sending HTTP request to the container\n");
    struct SendHttpGet_return response = SendHttpGet(containerId, 8080, GOSTRING("/hello"));
    if (response.r0 == -1) {
        printf("Failed to send HTTP request: %s\n", response.r2);
        return -1;
    }
    if (response.r0 != 200) {
        printf("Received wrong response code: %d instead of %d\n%s\n", response.r0, 200, response.r2);
        return -1;
    }
    printf("Server Response: HTTP-%d\n%s\n\n", response.r0, response.r1);
    return 0;
}


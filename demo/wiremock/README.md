# Demo - WireMock on Testcontainers C

Demonstrates usage of the [WireMock](https://wiremock.org/) module in a simple main function.
No test framework is used here.

## Running demo

From the root of the repository:

```bash
cmake --build .
cd demo/wiremock
./demo.out
```

## Sample output

![Sample Output](./sample_output.png)

## Wishlist

The library implementation is sub-optimal at the moment.
This is how we want to see the code once everything is done:

```c
#include <stdio.h>
#include <string.h>
#include "testcontainers-c.h"
#include "testcontainers-c-wiremock.h"

#define DEFAULT_IMAGE "wiremock/wiremock:3.1.0-1"

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_IMAGE);
    int requestId = NewContainerRequest(DEFAULT_IMAGE);
    WithExposedTcpPort(requestId, 8080);
    WithWaitForHttp(requestId, 8080, "/__admin/mappings");
    WithFile(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello.json");
    struct TestContainer container = RunContainer(requestId);
    if (container.Id == -1) {
        printf("Failed to run the container: %s\n", container.errorMsg);
        return -1;
    }

    printf("Sending HTTP request to the container\n");
    struct HttpGetResponse response = SendHttpGet(container.Id, 8080, "/hello");
    if (response.code == -1) {
        printf("Failed to send HTTP request: %s\n", response.errorMsg);
        return -1;
    }
    if (response.code != 200) {
        printf("Request failed: HTTP-%d\n%s\n", response.code, response.errorMsg);
        return -1;
    }
    printf("Server Response: HTTP-%d\n%s\n\n", response.code, response.msg);
    return 0;
}
```

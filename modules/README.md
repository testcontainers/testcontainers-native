# Testcontainers Native. Modules

As for other Testcontainers implementations, Testcontainers Native allows writing
extensions that extend the SDK and APIs to make usage of a particular service provider
easier.
The expectation is that the modules are implemented in a separate dynamic library
and linked to the consumer project.

The modules use Native API, and
can be a fully native implementation or
a Golang bridge one, similar to the core project.

## Why Modules?

Modules help to simplify test development and maintenance by encapsulating
domain-specific logic of a target container.
For example, the WireMock module adds an API to simplify the configuration of the container.
You can also use modules to create specific asserts for the container,
or even attach full-fledged API clients for fine-grain testing.

## Available Modules

The following modules are available for this project:

- Embedded Generic container for DYI containers - [DEMO](../demo/generic-container/README.md)
- [WireMock for API Mocking](./wiremock/README.md)

## Using Modules

Initializing WireMock with the module:

```c
#include "testcontainers-c-wiremock.h"

int main() {
    printf("Creating new container: %s\n", DEFAULT_WIREMOCK_IMAGE);
    int requestId = tc_wm_new_default_container();
    tc_wm_with_mapping(requestId, "test_data/hello.json", "hello");
    struct tc_run_container_return ret = tc_run_container(requestId);

    // ...
}
```

The same initialization without a module (using the "Generic Container" API):

<details>
<summary>
Show me the Code
</summary>

```c
#include "testcontainers-c.h"

#define DEFAULT_IMAGE "wiremock/wiremock:3.1.0-1"

int main() {
    printf("Using WireMock with the Testcontainers C binding:\n");

    printf("Creating new container: %s\n", DEFAULT_IMAGE);
    int requestId = tc_new_container_request(DEFAULT_IMAGE);
    tc_with_exposed_tcp_port(requestId, 8080);
    tc_with_wait_for_http(requestId, 8080, "/__admin/mappings");
    tc_with_file(requestId, "test_data/hello.json", "/home/wiremock/mappings/hello.json");
    struct tc_run_container_return ret = tc_run_container(requestId);

    // ...
}
```
</details>

## Creating New Modules

Just do it! Any pull requests are welcome,
and you can also create modules as standalone repositories.

### Where to Publish Modules?

Some modules are stored in [this repository](https://github.com/testcontainers/testcontainers-c) for demo and prototyping purposes.
You are welcome to add your modules there too.
If you develop new modules, once `vcpkg` or `Conan` packaging is implemented for Testcontainers C,
you might want to develop your module in a standalone repository instead.

# Architecture

!!! note
    This section is coming soon.
    All contributions are welcome, just submit a pull request!

## Build Process

![Testcontainers Native Build Process](./images/build-process.png)

### Stage 1. Testcontainers for C shared library

The core `testcontainers-c` shared library is built with [Cgo](https://pkg.go.dev/cmd/cgo).
For that the `-buildmode=c-shared` is used in the Golang builder,
and it also receives a customized header so that the types can be mapped between C and Golang.
For that, we also have to flatten the structure and to switch the reference-based build process to fixed Objects stored in the Golang namespace, and C API
using unique object IDs.

### Stage 2. Language Bindings

Then, we build custom binding libraries, by using Testcontainers for C
as a static library that is bundled into the language specific libs.
This stage depends on the language and common practices there.

### Stage 3. Executable Code

Well, you build it.
From the previous stages,
you get a header file, binding libraries,
a shared library object or a DLL file from the [Testcontainers for C](./docs/c/README.md) module.
Then, you know the drill.

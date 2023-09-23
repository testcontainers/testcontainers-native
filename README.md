# Testcontainers C & Shared Lib

> **WARNING**: This is a prototype that partially works,
> but there is a lot to do before it can be distributed
> and used in production.
> The plan is to build a proper distributable with a CMake proper structure,
> have proper Cpp Unit integrations,
> and have the library published to Conan.
> A feasible level of feature parity with Testcontainers Go is needed too,
> hence a lot of wrapper coding.
> Contributions are welcome!

This is not a standalone [Testcontainers](https://testcontainers.org/) engine,
but a C-style shared library adapter for native languages like C/C++, D, Lua, Swift, etc.
It is a MVP SDK that can be later extended for the languages.

The project is based on [Testcontainers for Go](https://golang.testcontainers.org/)
which is one of the most powerful Testcontainers implementations.

## Usage in C/C++

See [the examples](./testcontaincers-c/)

## Usage in other languages

TL;DR: You get a shared library object or a DLL file from [Testcontainers for Go. C-Shared](./testcontainers-go-c-shared/),
and also C header files. Then, you know the drill.

Feel free to contribute examples or SDKs for the languages.

## Examples

- [WireMock](./demo/wiremock/)

## Credits

- [Vladimir Vivien, Calling Go Functions from Other Languages using C Shared Libraries](https://github.com/vladimirvivien/go-cshared-examples)
- [Ben McClelland, An Adventure into CGO — Calling Go code with C](https://medium.com/@ben.mcclelland/an-adventure-into-cgo-calling-go-code-with-c-b20aa6637e75)
- https://insujang.github.io/2019-11-28/implementing-kubernetes-cpp-client-library

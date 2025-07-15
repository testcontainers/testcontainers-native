# Contributing

[![Slack: testcontainers-native on slack.testcontainers.com](https://img.shields.io/badge/Slack-%23testcontainers%E2%80%94native-brightgreen?style=flat&logo=slack)](https://slack.testcontainers.org/)
[![Stability: Experimental](https://masterminds.github.io/stability/experimental.svg)](https://masterminds.github.io/stability/experimental.html)

Contributions are welcome!
For any feedback and suggestions, use GitHub Issues.

## Community Slack

We use the `#testcontainers-native` channel on [Testcontainers Slack](https://slack.testcontainers.com/).
Please join this channel if you want to discuss contributions/strategy/whatever.
GitHub Issues are recommended for most of the proposals and bug reports though.

## Code patches

To propose a code patch, just submit a pull request to the repository.
No need to create a GitHub issue if you want to suggest a simple patch,
pull requests are more than enough for it.

## Building the project

You need CMake 3.16.3 and Golang 1.22.5+.
At the moment there are not so many configuration flags, so the build is straightforward:

```shell
cmake .
cmake --build .
ctest --output-on-failure
```

To skip the demo builds and tests, use the `SKIP_DEMOS` variable:

```shell
cmake -DSKIP_DEMOS=true .
```

## Contributing to the Documentation

The documentation is structured in the MkDocs format and uses Material for MkDocs.
To develop the site in this repository, start it in the [Dev Containers](.devcontainer/README.md)
and use the following commands:

```shell
mkdocs serve
```

```shell
mkdocs build
```

## Codestyle

### Public APIs

- `snake_case` is used to name methods
- `tc_` is used as a prefix for all exported Testcontainers functions
- When possible, we try to avoid special Golang types in public API and try to expose wrapper types
- `const` is important for users, and please add it to your arguments when possible.
  There is no Const in Golang, so some `typedef` injection is needed when importing CGo

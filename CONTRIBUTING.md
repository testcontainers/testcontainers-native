# Contributing

[![Slack: testcontainers-c on slacktestcontainers.org](https://img.shields.io/badge/Slack-%23testcontainers%E2%80%94c-brightgreen?style=flat&logo=slack)](http://slack.testcontainers.org/)
[![Stability: Experimental](https://masterminds.github.io/stability/experimental.svg)](https://masterminds.github.io/stability/experimental.html)

Contributions are welcome!
For any feedback and suggestions, use GitHub Issues.

## Community Slack

We use the `#testcontainers-c` channel on [Testcontainers Slack](http://slack.testcontainers.org/).
Please join this channel if you want to discuss contributions/strategy/whatever.
GitHub Issues are recommended for most of the proposals and bug reports though.

## Code patches

To propose a code patch, just submit a pull request to the repository.
No need to create a GitHub issue if you want to suggest a simple patch,
pull requests are more than enough for it.

## Building the project

You need CMake 3.16.3 and Golang 1.19+.
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

## Codestyle

### Public APIs

- `snake_case` is used to name methods
- `tc_` is used as a prefix for all exported Testcontainers functions
- When possible, we try to avoid special Golang types in public API and try to expose wrapper types
- `const` is important for users, and please add it to your arguments when possible.
  There is no Const in Golang, so some `typedef` injection is needed when importing CGo

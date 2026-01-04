# Demo - WireMocPostgreSQLk on Testcontainers C

Demonstrates usage of the [PostgreSQL Module for Testcontainers C](../../modules/postgresql/README.md) in a simple main function.
No test framework is used here.

## Running demo

First ensure `libpq` and/or `libpq-devel` is installed on your system - this is required to build the example.

From the root of the repository:

```bash
cmake -B build/
cmake --build build/
./build/demo/postgresql/demo_postgresql_module.out
```

The demo program will start a PostgreSQL container, use `libpq` to connect to it, and then terminate the container.

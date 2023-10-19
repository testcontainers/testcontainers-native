# Using Testcontainers C for OpenSCADA testing

Demonstrates usage of Testcontainers C in [Google Test](https://github.com/google/googletest).
See [test.cpp](./test.cpp) for the code.

## Run the demo

```bash
cmake .
cmake --build .
cd demo/google-test
ctest --output-on-failure
```

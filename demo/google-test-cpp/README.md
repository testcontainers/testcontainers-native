# Using Testcontainers C++ in Google Test

Demonstrates usage of Testcontainers C++ in [Google
Test](https://github.com/google/googletest). See
[test.cpp](./test.cpp) for the code.

## Run the demo

```bash
cmake -S . -B /tmp/tc-native
cmake --build /tmp/tc-native/
ctest --output-on-failure -R Class
```

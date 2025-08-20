# Using Testcontainers C in Google Test

Demonstrates usage of Testcontainers C in [Google Test](https://github.com/google/googletest).
See [test.cpp](./test.cpp) for the code.

## Run the demo

```bash
cmake .
cmake --build .
cd demo/google-test
ctest --output-on-failure
```

## Sample output

```shell
onenashev:~/testcontainers-c/demo/google-test$ ctest --output-on-failure
Test project /home/onenashev/testcontainers-c/demo/google-test
    Start 1: WireMockTestContainer.HelloWorld
1/5 Test #1: WireMockTestContainer.HelloWorld ......................   Passed    3.31 sec
    Start 2: WireMockTestContainer.HelloWorldFromResource
2/5 Test #2: WireMockTestContainer.HelloWorldFromResource ..........   Passed    3.97 sec
    Start 3: WireMockTestContainer.HelloWorldFromMissingResource
3/5 Test #3: WireMockTestContainer.HelloWorldFromMissingResource ...   Passed    3.91 sec

100% tests passed, 0 tests failed out of 3
```

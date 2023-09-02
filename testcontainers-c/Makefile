# Note: This is a lazy Makefile to speedup prototyping
# Proper Makefiles/CMake are yet to be set

build-testcontainers:
	go build -o testcontainers.so -buildmode=c-shared testcontainers-c.go

build-demo:
	gcc -o wiremock_hello_c wiremock_test.c ./testcontainers.so

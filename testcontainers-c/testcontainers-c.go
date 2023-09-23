package main

import "C"

import (
	"context"
	"fmt"
	"io"
	"net/http"
	"strconv"

	"github.com/docker/go-connections/nat"
	"github.com/testcontainers/testcontainers-go"
	"github.com/testcontainers/testcontainers-go/wait"
)

var containerRequests []*testcontainers.ContainerRequest
var containers []*testcontainers.Container
var customizers map[int][]*testcontainers.CustomizeRequestOption

// Creates Unique container request and returns its ID
//
//export tc_new_container_request
func tc_new_container_request(image *C.char) (id int) {
	req := testcontainers.ContainerRequest{
		Image: C.GoString(image),
		Cmd:   []string{""},
	}

	containerRequests = append(containerRequests, &req)
	return len(containerRequests) - 1
}

//export tc_run_container
func tc_run_container(requestID int) (id int, errstr *C.char) {
	id, err := _RunContainer(requestID)
	if err != nil {
		return -1, ToCString(err)
	}
	return id, nil
}

func _RunContainer(requestID int) (id int, err error) {
	req := *containerRequests[requestID]
	ctx := context.Background()

	genericContainerReq := testcontainers.GenericContainerRequest{
		ContainerRequest: req,
		Started:          true,
	}

	for _, opt := range customizers[requestID] {
		opt.Customize(&genericContainerReq)
	}

	container, err := testcontainers.GenericContainer(ctx, genericContainerReq)
	if err != nil {
		return -1, err
	}

	containers = append(containers, &container)
	return len(containers) - 1, nil
}

//export tc_get_uri
func tc_get_uri(containerID int, port int) (uri string, e error) {
	ctx := context.Background()
	container := *containers[containerID]
	return _GetURI(ctx, container, port)
}

func _GetURI(ctx context.Context, container testcontainers.Container, port int) (string, error) {
	hostIP, err := container.Host(ctx)
	if err != nil {
		return "", err
	}

	mappedPort, err := container.MappedPort(ctx, nat.Port(strconv.Itoa(port)))
	if err != nil {
		return "", fmt.Errorf("Cannot retrieve mappedPort for %d: %w", port, err)
	}

	return "http://" + hostIP + ":" + mappedPort.Port(), nil
}

//export tc_with_wait_for_http
func tc_with_wait_for_http(requestID int, port int, url string) {
	req := func(req *testcontainers.GenericContainerRequest) {
		req.WaitingFor = wait.ForHTTP(url).WithPort(nat.Port(strconv.Itoa(port)))
	}

	registerCustomizer(requestID, req)
}

//export tc_with_file
func tc_with_file(requestID int, filePath *C.char, targetPath *C.char) {
	req := func(req *testcontainers.GenericContainerRequest) {
		cfgFile := testcontainers.ContainerFile{
			HostFilePath:      C.GoString(filePath),
			ContainerFilePath: C.GoString(targetPath),
			FileMode:          0755,
		}
		req.Files = append(req.Files, cfgFile)
	}

	registerCustomizer(requestID, req)
}

//export tc_with_exposed_tcp_port
func tc_with_exposed_tcp_port(requestID int, port int) {
	req := func(req *testcontainers.GenericContainerRequest) {
		req.ExposedPorts = append(req.ExposedPorts, strconv.Itoa(port)+"/tcp")
	}

	registerCustomizer(requestID, req)
}

func registerCustomizer(requestID int, customizer testcontainers.CustomizeRequestOption) int {
	if customizers == nil {
		customizers = make(map[int][]*testcontainers.CustomizeRequestOption)
	}
	customizers[requestID] = append(customizers[requestID], &customizer)
	return len(customizers[requestID]) - 1
}

//export tc_send_http_get
func tc_send_http_get(containerID int, port int, endpoint string) (responseCode C.int, responseBody *C.char, errstr *C.char) {
	container := *containers[containerID]
	responseCodeVal, responseBodyStr, err := SendHttpRequest(http.MethodGet, container, port, endpoint, nil)
	if err != nil {
		return -1, nil, ToCString(err)
	}

	return C.int(responseCodeVal), C.CString(responseBodyStr), nil
}

func SendHttpRequest(httpMethod string, container testcontainers.Container, port int, endpoint string, body io.Reader) (responseCode int, responseBody string, err error) {
	ctx := context.Background()

	uri, err := _GetURI(ctx, container, port)
	if err != nil {
		return -1, "", err
	}

	req, err := http.NewRequest(httpMethod, uri+endpoint, body)
	if err != nil {
		return -1, "", err
	}

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return -1, "", err
	}

	out, err := io.ReadAll(res.Body)
	if err != nil {
		return -1, "", err
	}

	return res.StatusCode, string(out), nil
}

func ToCString(err error) *C.char {
	return C.CString(fmt.Sprintf("%v", err))
}

func main() {}

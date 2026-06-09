package main

// #include <stdbool.h>
// #include <stddef.h>
// typedef const char cchar_t;
// typedef bool (*exit_code_matcher)(int);
// static bool invoke_matcher(exit_code_matcher f, int x) { return f(x); }
import "C"

import (
	"context"
	"fmt"
	"io"
	"net/http"
	"strconv"
	"unsafe"

	"github.com/docker/go-connections/nat"
	"github.com/testcontainers/testcontainers-go"
	"github.com/testcontainers/testcontainers-go/wait"
)

var containerRequests []*testcontainers.ContainerRequest
var containers []*testcontainers.Container
var customizers map[int][]*testcontainers.CustomizeRequestOption
var execStrategies map[int]*wait.ExecStrategy

// Creates Unique container request and returns its ID
//
//export tc_bridge_new_container_request
func tc_bridge_new_container_request(image *C.cchar_t) (id int) {
	req := testcontainers.ContainerRequest{
		Image: C.GoString(image),
	}

	containerRequests = append(containerRequests, &req)
	return len(containerRequests) - 1
}

//export tc_bridge_run_container
func tc_bridge_run_container(requestID int) (id int, ok bool, errstr *C.char) {
	id, ok, err := _RunContainer(requestID)
	if err != nil {
		return -1, ok, ToCString(err)
	}
	return id, ok, nil
}

func _RunContainer(requestID int) (id int, ok bool, err error) {
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
	containerId := -1
	if container != nil {
		// We register the container even if the run failed
		containers = append(containers, &container)
		containerId = len(containers) - 1
	}
	if err != nil {
		return containerId, false, err
	}

	containers = append(containers, &container)
	return containerId, true, nil
}

//export tc_bridge_terminate_container
func tc_bridge_terminate_container(containerID int) *C.char {
	ctx := context.Background()
	container := *containers[containerID]
	return ToCString(container.Terminate(ctx))
}

//export tc_bridge_get_container_log
func tc_bridge_get_container_log(containerID int) (log *C.char) {
	ctx := context.Background()
	container := *containers[containerID]

	logs, err := container.Logs(ctx)
	if err != nil {
		return nil
	}

	bytes, err := io.ReadAll(logs)
	if err != nil {
		return nil
	}

	//FIXME: Returning ERR here results in panic with "invalid memory address or nil pointer dereference" in CgoCheckResult
	return C.CString(string(bytes))
}

//export tc_bridge_get_uri
func tc_bridge_get_uri(containerID int, port int) (uri *C.char, ok bool, errstr *C.char) {
	ctx := context.Background()
	container := *containers[containerID]
	str, err := _GetURI(ctx, container, port)
	if err != nil {
		return nil, false, ToCString(err)
	}
	return C.CString(str), true, nil
}

//export tc_bridge_get_mapped_port
func tc_bridge_get_mapped_port(containerID int, containerPort int) (mappedPort int, ok bool, errstr *C.char) {
	ctx := context.Background()
	container := *containers[containerID]

	goPort, err := container.MappedPort(ctx, nat.Port(strconv.Itoa(containerPort)))
	if err != nil {
		return -1, false, ToCString(err)
	}

	return goPort.Int(), true, nil
}

//export tc_bridge_get_hostname
func tc_bridge_get_hostname(containerID int) (hostname *C.char, ok bool, errstr *C.char) {
	ctx := context.Background()
	container := *containers[containerID]

	hostIP, err := container.Host(ctx)
	if err != nil {
		return nil, false, ToCString(err)
	}

	return C.CString(hostIP), true, nil
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

//export tc_bridge_with_wait_for_http
func tc_bridge_with_wait_for_http(requestID int, port int, url *C.cchar_t) {
	req := func(req *testcontainers.GenericContainerRequest) error {
		req.WaitingFor = wait.ForHTTP(C.GoString(url)).WithPort(nat.Port(strconv.Itoa(port)))
        return nil
	}

	registerCustomizer(requestID, req)
}

//export tc_bridge_with_wait_for_exec
func tc_bridge_with_wait_for_exec(requestID int, cmd **C.cchar_t, cmdLen C.size_t) (strategyID int) {
	convertedCmd := ToGoStringSlice(cmd, cmdLen)
	strategy := wait.ForExec(convertedCmd)

	if execStrategies == nil {
		execStrategies = make(map[int]*wait.ExecStrategy)
	}
	execStrategies[requestID] = strategy

	req := func(req *testcontainers.GenericContainerRequest) error {
		req.WaitingFor = strategy
		return nil
	}

	registerCustomizer(requestID, req)

	return requestID
}

//export tc_bridge_exec_with_exit_code_matcher
func tc_bridge_exec_with_exit_code_matcher(strategyID int, matcher C.exit_code_matcher) {
	strategy := execStrategies[strategyID]

	execStrategies[strategyID] = strategy.WithExitCodeMatcher(func(exitCode int) bool {
		result := C.invoke_matcher(matcher, C.int(exitCode))
		return bool(result)
	})
}

//export tc_bridge_with_file
func tc_bridge_with_file(requestID int, filePath *C.cchar_t, targetPath *C.cchar_t) {
	req := func(req *testcontainers.GenericContainerRequest) error {
		cfgFile := testcontainers.ContainerFile{
			HostFilePath:      C.GoString(filePath),
			ContainerFilePath: C.GoString(targetPath),
			FileMode:          0755,
		}
		req.Files = append(req.Files, cfgFile)
        return nil
	}

	registerCustomizer(requestID, req)
}

//export tc_bridge_with_exposed_tcp_port
func tc_bridge_with_exposed_tcp_port(requestID int, port int) {
	req := func(req *testcontainers.GenericContainerRequest) error {
		req.ExposedPorts = append(req.ExposedPorts, strconv.Itoa(port)+"/tcp")
        return nil
	}

	registerCustomizer(requestID, req)
}

//export tc_bridge_with_env
func tc_bridge_with_env(requestID int, name *C.cchar_t, value *C.cchar_t) {
	req := func(req *testcontainers.GenericContainerRequest) error {
		if req.Env == nil {
			req.Env = make(map[string]string)
		}
		req.Env[C.GoString(name)] = C.GoString(value)
		return nil
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

//export tc_bridge_send_http_get
func tc_bridge_send_http_get(containerID int, port int, endpoint *C.cchar_t) (responseCode C.int, responseBody *C.char, errstr *C.char) {
	container := *containers[containerID]
	responseCodeVal, responseBodyStr, err := SendHttpRequest(http.MethodGet, container, port, C.GoString(endpoint), nil)
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
	if err != nil {
		return C.CString(fmt.Sprintf("%v", err))
	}
	return nil
}

func ToGoStringSlice(arrayStart **C.cchar_t, arrayLen C.size_t) []string {
	outputSlice := make([]string, arrayLen)
	inputSlice := unsafe.Slice(arrayStart, arrayLen)

	for idx, cStr := range inputSlice {
		outputSlice[idx] = C.GoString(cStr)
	}

	return outputSlice
}

func main() {}

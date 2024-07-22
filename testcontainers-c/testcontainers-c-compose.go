package main

// typedef const char cchar_t;
import "C"

import (
	"context"
	"fmt"
	"io"
	"net/http"
	"strconv"
	"strings"

	"github.com/docker/go-connections/nat"
	"github.com/testcontainers/testcontainers-go"

	uuid "github.com/google/uuid"
	compose "github.com/testcontainers/testcontainers-go/modules/compose"
	"github.com/testcontainers/testcontainers-go/wait"
)

var composeGroups []compose.ComposeStack

//export tc_new_docker_compose
func tc_new_docker_compose(path *C.cchar_t) (id int, ok bool, errstr *C.char) {
	composePath := C.GoString(path)
	composeInstance, err := compose.NewDockerComposeWith(compose.WithStackFiles(composePath))
	if err != nil {
		return -1, false, C.CString(fmt.Errorf("Cannot initialize Docker Compose for %q: %w", path, err).Error())
	}

	composeId := -1
	if composeInstance != nil {
		// We register the container even if the run failed
		composeGroups = append(composeGroups, &composeInstance)
		composeId = len(containers) - 1
	}
	return composeId, true, nil
}

//export tc_terminate_compose
func tc_terminate_compose(composeID int) *C.char {
	ctx := context.Background()
	composeInstance := composeGroups[composeID]
	res := composeInstance.Down(ctx, compose.RemoveOrphans(true), compose.RemoveImagesLocal)
	return ToCString(res)
}

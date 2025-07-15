package main

// typedef const char cchar_t;
import "C"

import (
	"context"
	"fmt"
	"sync"

	"github.com/testcontainers/testcontainers-go"

	"github.com/compose-spec/compose-go/v2/cli"
	"github.com/compose-spec/compose-go/v2/types"
	"github.com/docker/compose/v2/pkg/api"
	"github.com/docker/docker/client"
	compose "github.com/testcontainers/testcontainers-go/modules/compose"
	"github.com/testcontainers/testcontainers-go/wait"
)

var composeGroups []*DockerComposeRequest

// FIXME: Copy of compose_api: dockerCompose. Export to public
type DockerComposeContext struct {
	stack			ComposeStack *
	terminator  	func() error 
}

//export tc_new_docker_compose
func tc_run_docker_compose(path *C.cchar_t) (id int, ok bool, errstr *C.char) {
	composePath := C.GoString(path)
	composeInstance, err := compose.NewDockerComposeWith(compose.WithStackFiles(composePath))
	if err != nil {
		return -1, false, C.CString(fmt.Errorf("Cannot initialize Docker Compose for %q: %w", path, err).Error())
	}

	composeId := -1
	request := DockerComposeContext()
	if composeInstance != nil {
		terminator := func() error {
			return composeInstance.Down(context.Background(), compose.RemoveOrphans(true), compose.RemoveImagesLocal)
		}

		request.terminator = terminator
		stack := composeInstance.Up(ctx, tc.WithRecreate(api.RecreateNever), tc.Wait(true))
		composeInstance.stack = &stack;

		// We register the container even if the run failed
		composeGroups = append(composeGroups, request)
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

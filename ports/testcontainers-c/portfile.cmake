# Build using the port directory as source. We expect this repository to be
# available to the vcpkg build as a submodule or local path. If building from
# a remote registry, replace this with vcpkg_from_github / vcpkg_from_git.

set(ROOT_SRC ${CURRENT_PORT_DIR}/../../testcontainers-c)


# Configure from repository root so that testcontainers-bridge is built first
vcpkg_cmake_configure(
    SOURCE_PATH ${ROOT_SRC}
    GENERATOR "Unix Makefiles"
    OPTIONS
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

vcpkg_build_cmake()

# Install from CMake install rules
vcpkg_install_cmake()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

# Add a short README to the package to indicate local build origin
file(WRITE ${CURRENT_PACKAGES_DIR}/debug/share/${PORT}/README "This package was built from local repo source.\n")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

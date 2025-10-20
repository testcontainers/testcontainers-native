vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO MicrosoftDocs/vcpkg-docs
    REF "1.0.2"
    SHA512 4202125968a01219deeee14b81e1d476dab18d968425ba36d640816b0b3db6168f8ccf4120ba20526e9930c8c7294e64d43900ad2aef9d5f28175210d0c3a417
    HEAD_REF main
)

# Build using the port directory as source. We expect this repository to be
# available to the vcpkg build as a submodule or local path. If building from
# a remote registry, replace this with vcpkg_from_github / vcpkg_from_git.

set(ROOT_SRC ${CURRENT_PORT_DIR}/../../)


# Configure from repository root so that testcontainers-bridge is built first
vcpkg_configure_cmake(
    SOURCE_PATH ${ROOT_SRC}
    PREFER_NINJA
    OPTIONS
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DSKIP_DEMOS=true
)

vcpkg_build_cmake()

# Install from CMake install rules
vcpkg_install_cmake()

vcpkg_cmake_config_fixup(PACKAGE_NAME "testcontainers-c")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

# Add a short README to the package to indicate local build origin
file(WRITE ${CURRENT_PACKAGES_DIR}/debug/share/${PORT}/README "This package was built from local repo source.\n")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

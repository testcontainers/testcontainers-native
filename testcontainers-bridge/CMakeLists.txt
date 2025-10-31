cmake_minimum_required (VERSION 3.26)
project (TESTCONTAINERS-BRIDGE
    VERSION 0.1.0
    DESCRIPTION "Testcontainers Go library for native languages"
    LANGUAGES C
)
include(GNUInstallDirs)

set(SRCS testcontainers-bridge.go)

set(SHIM_TARGET testcontainers-bridge-shim)
set(SHIM_TARGET_LIB testcontainers-bridge.so)
set(SHIM_TARGET_HEADER testcontainers-bridge.h)

add_custom_command(OUTPUT ${SHIM_TARGET_LIB} ${SHIM_TARGET_HEADER}
  DEPENDS ${SRCS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND env GOPATH=${GOPATH} go build -buildmode=c-archive -linkshared
  -o "${CMAKE_CURRENT_BINARY_DIR}/${SHIM_TARGET_LIB}"
  ${CMAKE_GO_FLAGS} ${SRCS}
  COMMENT "Building Testcontainers Go to C bridge library")

add_custom_target(${SHIM_TARGET} DEPENDS ${SHIM_TARGET_LIB} ${SHIM_TARGET_HEADER})

set(TARGET testcontainers-bridge)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "Go to C bridge for Testcontainers Go functionality")
set(TARGET_VERSION ${PROJECT_VERSION})

add_library(${TARGET} STATIC IMPORTED GLOBAL)
add_dependencies(${TARGET} ${SHIM_TARGET})
set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE C
    VERSION ${TARGET_VERSION}
    PUBLIC_HEADER ${SHIM_TARGET_HEADER}
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${SHIM_TARGET_LIB}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})

# Install library and header files
configure_file(cmake.pc.in ${TARGET}.pc @ONLY)
install(FILES ${SHIM_TARGET_LIB} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${SHIM_TARGET_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${TARGET}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

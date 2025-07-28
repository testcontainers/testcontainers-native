cmake_minimum_required(VERSION 3.0)
project(testcontainers-native VERSION 0.0.1
    DESCRIPTION "Testcontainers Native library")

include(GNUInstallDirs)

set(SRCS testcontainers-native.go)

set(TARGET testcontainers-native-shim)
set(TARGET_LIB testcontainers-native.so)
set(TARGET_HEADER testcontainers-native.h)

add_custom_command(OUTPUT ${TARGET_LIB} ${TARGET_HEADER}
  DEPENDS ${SRCS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND env GOPATH=${GOPATH} go build -buildmode=c-shared
  -o "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB}"
  ${CMAKE_GO_FLAGS} ${SRCS}
  COMMENT "Building Testcontainers Native shared library")

add_custom_target(${TARGET} DEPENDS ${TARGET_LIB} ${TARGET_HEADER})

add_library(${PROJECT_NAME} SHARED IMPORTED GLOBAL)
add_dependencies(${PROJECT_NAME} ${TARGET})
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE C
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER testcontainers-native.h
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})

configure_file(cmake.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${TARGET_LIB}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${TARGET_HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

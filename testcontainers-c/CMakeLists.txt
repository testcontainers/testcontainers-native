cmake_minimum_required(VERSION 3.0)
project(testcontainers-c VERSION 0.0.1
    DESCRIPTION "Testcontainers C library")

include(GNUInstallDirs)

set(SRCS testcontainers-c.go)

set(TARGET testcontainers-c-shim)
set(TARGET_LIB testcontainers-c.so)
set(TARGET_HEADER testcontainers-c.h)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB}
  DEPENDS ${SRCS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND env GOPATH=${GOPATH} go build -buildmode=c-shared
  -o "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB}"
  ${CMAKE_GO_FLAGS} ${SRCS}
  COMMENT "Building Testcontainers C shared library")

add_custom_target(${TARGET} DEPENDS ${TARGET_LIB} ${TARGET_HEADER})

add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB})
link_directories(${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(${PROJECT_NAME} ${TARGET})
set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE C
    VERSION ${PROJECT_VERSION}    
    PUBLIC_HEADER testcontainers-c.h)

configure_file(cmake.pc.in ${PROJECT_NAME}.pc @ONLY)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)




# TODO: goshim
#add_dependencies(goshim ${TARGET_LIB})
#add_library(goshim DYNAMIC IMPORTED GLOBAL)
#set_target_properties(goshim
#  PROPERTIES
#  IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB}
#  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})


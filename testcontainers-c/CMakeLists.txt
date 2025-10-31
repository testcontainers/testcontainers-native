cmake_minimum_required (VERSION 3.26)
project (TESTCONTAINERS-C
    VERSION 0.1.0
    DESCRIPTION "Testcontainers library for C"
    LANGUAGES C
)
include(GNUInstallDirs)

set(TARGET testcontainers-c)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "Testcontainers library for C")
set(TARGET_VERSION ${PROJECT_VERSION})

# Try to find testcontainers-bridge via find_package (for vcpkg/installed case)
# If not found, assume it's built in the same project (source build)
find_package(testcontainers-bridge QUIET)
if(NOT testcontainers-bridge_FOUND)
    if(NOT TARGET testcontainers-bridge)
        message(FATAL_ERROR "testcontainers-bridge not found. Will assume the local presence.")
    endif()
endif()

add_library(${TARGET} SHARED
    src/container.c
)

target_sources(${TARGET}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS include
    FILES include/testcontainers-c/container.h
)

target_link_libraries(${TARGET} PRIVATE testcontainers-bridge)

configure_file(cmake.pc.in ${TARGET}.pc @ONLY)
install(FILES ${SHIM_TARGET_LIB} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${SHIM_TARGET_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${TARGET}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

set(TARGET testcontainers-c)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "Testcontainers library for C")
set(TARGET_VERSION ${PROJECT_VERSION})

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
install(FILES ${SHIM_TARGET_LIB}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${SHIM_TARGET_HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${TARGET}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

set(TARGET testcontainers-c-wiremock)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "WireMock Testcontainers abstractions for C")
set(TARGET_VERSION ${PROJECT_VERSION})

add_library(${TARGET} SHARED
    impl.c
)

target_sources(${TARGET}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS .
    FILES testcontainers-c-wiremock.h
)

target_link_libraries(${TARGET} PRIVATE testcontainers-c)

configure_file(cmake.pc.in ${TARGET}.pc @ONLY)
install(TARGETS ${TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${TARGET}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

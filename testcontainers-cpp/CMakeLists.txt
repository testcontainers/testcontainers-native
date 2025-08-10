set(TARGET testcontainers-cpp)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "Testcontainer library for C++")
set(TARGET_VERSION ${PROJECT_VERSION})

add_library(${TARGET} INTERFACE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_sources(${TARGET}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS .
    FILES testcontainers.hpp
)

target_link_libraries(${TARGET} INTERFACE testcontainers-c)

configure_file(cmake.pc.in ${TARGET}.pc @ONLY)
install(TARGETS ${TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${TARGET}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

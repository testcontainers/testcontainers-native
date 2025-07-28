
cmake_minimum_required(VERSION 3.9)
project(testcontainers-native-wiremock VERSION 0.0.1
    DESCRIPTION "WireMock module for Testcontainers Native")

include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED
    testcontainers-native-wiremock.h
    impl.c
)
add_dependencies(${PROJECT_NAME} testcontainers-native)

include_directories(${testcontainers-native_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER testcontainers-native-wiremock.h)

configure_file(cmake.pc.in ${PROJECT_NAME}.pc @ONLY)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

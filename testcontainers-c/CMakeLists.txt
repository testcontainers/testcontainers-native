set(TARGET testcontainers-c)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "Testcontainer library for C")
set(TARGET_VERSION ${PROJECT_VERSION})

add_library(${TARGET} SHARED
    src/container.c
)

target_include_directories(${TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${TARGET} PRIVATE testcontainers-bridge)

install(TARGETS ${TARGET} EXPORT ${TARGET}Targets)
install(FILES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${TARGET}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}Config.cmake
    INSTALL_DESTINATION lib/cmake/${TARGET}
)

install(EXPORT ${TARGET}Targets
    FILE ${TARGET}Config.cmake
    NAMESPACE testcontainers-native::
    DESTINATION lib/cmake/${TARGET}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}Config.cmake
    DESTINATION lib/cmake/${TARGET}
)

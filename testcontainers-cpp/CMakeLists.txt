set(TARGET testcontainers-cpp)
set(TARGET_NAME ${TARGET})
set(TARGET_DESCRIPTION "Testcontainer library for C++")
set(TARGET_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${TARGET} INTERFACE)
target_include_directories(${TARGET} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${TARGET} INTERFACE testcontainers-c)

install(TARGETS ${TARGET} EXPORT ${TARGET}Targets)
install(FILES testcontainers.hpp DESTINATION include)

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

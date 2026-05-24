cmake_minimum_required (VERSION 3.26)
project (postgresql-demo
    VERSION 0.1.0
    DESCRIPTION "Demonstrates usage of the PostgreSQL module for Testcontainers C in a simple main app"
    LANGUAGES C
)

set(TARGET_OUT demo_postgresql_module.out)

include(FindPostgreSQL)

# v17: use a lower version of the client library as not many distros ship libpq v18.x. 
# Client-server communication is fully backwards compatible unless we want to use new client features.
find_package(PostgreSQL 17) 

if (${PostgreSQL_FOUND})
    add_executable(${TARGET_OUT} postgresql_module_demo.c)
    target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c)
    target_link_libraries(${TARGET_OUT} PRIVATE testcontainers-c-postgresql)
    target_link_libraries(${TARGET_OUT} PRIVATE ${PostgreSQL_LIBRARIES})
else()
    message(WARNING "Could not find PostgreSQL client libraries. Skipping build of PostgreSQL demo. To fix this, try installing libpq and/or libpq-devel.")
endif()
cmake_minimum_required(VERSION 3.4)
project(squeal)
option(ENABLE_TESTS "Enable Tests" OFF)

if (${ENABLE_TESTS})
    enable_testing()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Threads REQUIRED)

include_directories(include src/sql)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -std=c99")

set(SQUEAL_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/squeal.c
        ${CMAKE_SOURCE_DIR}/src/squeal_string.c
        ${CMAKE_SOURCE_DIR}/src/squeal_threads.c
        ${CMAKE_SOURCE_DIR}/src/squeal_hashtable.c
        ${CMAKE_SOURCE_DIR}/src/squeal_function.c
        ${CMAKE_SOURCE_DIR}/src/squeal_backend.c
        ${CMAKE_SOURCE_DIR}/src/squeal_result.c
        ${CMAKE_SOURCE_DIR}/src/squeal_config.c
        ${CMAKE_SOURCE_DIR}/src/squeald.h)

set(SQUEALD_SOURCE_FILES ${SQUEAL_SOURCE_FILES} src/squeald.c)
set_property(GLOBAL PROPERTY G_SQUEAL_SOURCE_FILES ${SQUEAL_SOURCE_FILES})

if (${ENABLE_TESTS})
    add_subdirectory(tests)
endif()

add_executable(squeald ${SQUEALD_SOURCE_FILES})

install(TARGETS squeald DESTINATION bin)
install(FILES LICENSE.md DESTINATION .)
install(DIRECTORY
        include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")
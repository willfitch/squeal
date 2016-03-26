cmake_minimum_required(VERSION 3.4)
project(squeal)

include_directories(include src/sql)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

set(SOURCE_FILES
        src/squeal.c
        src/squeal_string.c
        src/squeal_threads.c
        src/squeald.c
        src/squeal_hashtable.c
        src/squeald.h)

add_executable(squeald ${SOURCE_FILES})
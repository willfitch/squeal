cmake_minimum_required(VERSION 3.4)
project(squeal)

include_directories(include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

set(SOURCE_FILES
        src/squeal.c
        src/squeal_string.c
        src/squeald.c
        src/squeald.h)

#add_library(mysqlevent-static STATIC ${SOURCE_FILES})
#set_target_properties(mysqlevent-static PROPERTIES OUTPUT_NAME mysqlevent CLEAN_DIRECT_OUTPUT 1)
#add_library(mysqlevent-shared MODULE ${SOURCE_FILES})
#set_target_properties(mysqlevent-shared PROPERTIES OUTPUT_NAME mysqlevent CLEAN_DIRECT_OUTPUT 1)

add_executable(squeald ${SOURCE_FILES})
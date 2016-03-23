cmake_minimum_required(VERSION 3.4)
project(libmysqlevent)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/lme_global.h
    include/lme_string.h
    include/lme_types.h
    src/libmyevent.c
    src/lme_string.c)

add_library(mysqlevent-static STATIC ${SOURCE_FILES})
set_target_properties(mysqlevent-static PROPERTIES OUTPUT_NAME mysqlevent CLEAN_DIRECT_OUTPUT 1)
add_library(mysqlevent-shared SHARED ${SOURCE_FILES})
set_target_properties(mysqlevent-shared PROPERTIES OUTPUT_NAME mysqlevent CLEAN_DIRECT_OUTPUT 1)
add_library(mysqlevent-mod MODULE ${SOURCE_FILES})
set_target_properties(mysqlevent-mod PROPERTIES OUTPUT_NAME mysqlevent CLEAN_DIRECT_OUTPUT 1)

#add_executable(libmysqlevent ${SOURCE_FILES})
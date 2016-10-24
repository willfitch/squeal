get_property(SQUEAL_SOURCE_FILES GLOBAL PROPERTY G_SQUEAL_SOURCE_FILES)
find_package(Check REQUIRED)

include_directories(${CHECK_INCLUDE_DIR})

set(SQUEAL_TEST_FILES runner.c)

file(COPY test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(squeal_tests ${SQUEAL_SOURCE_FILES} ${SQUEAL_TEST_FILES})
target_link_libraries(squeal_tests ${CHECK_LIBRARIES})
add_test(NAME squeal_tests COMMAND squeal_tests)
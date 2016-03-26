get_property(SQUEAL_SOURCE_FILES GLOBAL PROPERTY G_SQUEAL_SOURCE_FILES)
find_package(LibCheck REQUIRED)

set(SQUEAL_TEST_FILES runner.c)

add_executable(squeal_tests ${SQUEAL_SOURCE_FILES} ${SQUEAL_TEST_FILES})
target_link_libraries(squeal_tests ${LIBCHECK_LIBRARY} ${LIBCHECK_LDFLAGS})
add_test(NAME squeal_tests COMMAND squeal_tests)
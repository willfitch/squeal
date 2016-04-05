
file(GLOB SRC_FILES *.c *.h)
add_library(drizzle STATIC ${SRC_FILES})
install(TARGETS drizzle DESTINATION lib)
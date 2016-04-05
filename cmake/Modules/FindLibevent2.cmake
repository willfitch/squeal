if ("$ENV{STATIC}" STREQUAL "")
	    if (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
	        set(LIBEVENT2_FOUND TRUE)
	    else (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
	        find_path(LIBEVENT2_INCLUDE_DIRS
	            NAMES
	                event2/dns.h
	            PATHS
	                ${LIBEVENT2_PREFIX}/include
	                /usr/include
	                /usr/local/include
	                /opt/local/include
	                ${CMAKE_BINARY_DIR}/libevent2-bin/include
	            NO_DEFAULT_PATH
	        )

	        find_library(LIBEVENT2_LIBRARIES
	            NAMES
	                event
	            PATHS
	                ${LIBEVENT2_INCLUDE_DIRS}/../lib
	                ${LIBEVENT2_PREFIX}/lib
	                /usr/lib
	                /usr/local/lib
	                /opt/local/lib
	            NO_DEFAULT_PATH
	        )
	        if(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
	           set(LIBEVENT2_FOUND TRUE)
	        endif(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
	    endif (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
endif ("$ENV{STATIC}" STREQUAL "")
cmake_minimum_required(VERSION 3.16)

project(threadpool)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Option to build tests
option(THREADPOOL_BUILD_TESTS "Build threadpool tests" OFF)

# Threadpool library
add_library(threadpool
    src/threadpool.c
)

# Include headers
target_include_directories(threadpool PUBLIC include)

# Platform specific thread implementation
if(WIN32)
    target_sources(threadpool PRIVATE src/thread_windows.c)
else()
    target_sources(threadpool PRIVATE src/thread_linux.c)
    target_link_libraries(threadpool pthread)
endif()

# Test executable
if(THREADPOOL_BUILD_TESTS)
	add_executable(test_threadpool test/test_threadpool.c)

	target_link_libraries(test_threadpool threadpool)
endif()

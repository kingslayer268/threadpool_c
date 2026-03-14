cmake_minimum_required(VERSION 3.16)

project(threadpool C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Prefer Ninja if available
if(NOT CMAKE_GENERATOR)
    find_program(NINJA_EXECUTABLE ninja)
    if(NINJA_EXECUTABLE)
        set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
    endif()
endif()

# Include headers
include_directories(include)

# Threadpool library
add_library(threadpool
    src/threadpool.c
)

# Platform specific thread implementation
if(WIN32)
    target_sources(threadpool PRIVATE src/thread_windows.c)
else()
    target_sources(threadpool PRIVATE src/thread_linux.c)
    target_link_libraries(threadpool pthread)
endif()

# Test executable
add_executable(test_threadpool
    test/test_threadpool.c
)

target_link_libraries(test_threadpool threadpool)

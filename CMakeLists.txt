cmake_minimum_required(VERSION 3.10...4.0)
project(testlib)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c src/*.hpp src/*.h)

add_library(testlib SHARED ${SOURCES})

find_package(OpenMP QUIET)
if (OpenMP_FOUND)
    MESSAGE(STATUS "Found OpenMP version ${OpenMP_CXX_VERSION}. Enabling OpenMP threading support.")
else()
    MESSAGE(STATUS "OpenMP not found. Disabling OpenMP threading support.")
endif()
if (OpenMP_FOUND)
    target_link_libraries(testlib PRIVATE OpenMP::OpenMP_CXX)
endif()

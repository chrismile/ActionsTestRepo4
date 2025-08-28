cmake_minimum_required(VERSION 3.10...4.0)
project(testlib)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c src/*.hpp src/*.h)

add_library(testlib SHARED ${SOURCES})

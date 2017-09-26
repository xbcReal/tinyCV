cmake_minimum_required(VERSION 2.8.7)
project(tinyCV)
include_directories(core)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

set(CMAKE_CXX_FLAGS -std=c++11)

ADD_EXECUTABLE(test_matrix test/test_matrix.cpp)
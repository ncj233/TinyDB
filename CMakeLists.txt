cmake_minimum_required(VERSION 3.5)
project(TinyDB)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES src/main.cpp)
add_executable(TinyDB ${SOURCE_FILES})

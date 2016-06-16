cmake_minimum_required(VERSION 3.5)
project(TinyDB)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast -std=c++14")

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB TINYDB_SOURCE_FILES
    "src/*/*.h"
    "src/*/*.cpp"
    "src/*/*.cc"
)

add_executable(TinyDB src/main.cpp ${TINYDB_SOURCE_FILES})

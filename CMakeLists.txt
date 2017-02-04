cmake_minimum_required(VERSION 3.3)
project(teeton)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/enums.h
        src/environment.h
        src/environment.cpp
        src/main.cpp
        src/node.h
        src/node.cpp
        src/type.cpp
        src/type.h
        src/utils.h
        src/utils.cpp
        )
add_executable(teeton ${SOURCE_FILES})
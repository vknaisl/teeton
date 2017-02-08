cmake_minimum_required(VERSION 3.3)
project(teeton)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/enums.h
        src/environment.h
        src/environment.cpp
        src/lexer.h
        src/lexer.cpp
        src/main.cpp
        src/node.h
        src/node.cpp
        src/scanner.h
        src/scanner.cpp
        src/type.cpp
        src/type.h
        src/utils.h
        src/utils.cpp src/parser.cpp src/parser.h)
add_executable(teeton ${SOURCE_FILES})
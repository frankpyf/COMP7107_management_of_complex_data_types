cmake_minimum_required(VERSION 3.25)

project(COMP7107
LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

add_executable(assignment1)
target_sources(assignment1
    PRIVATE
        src/assignment1/assignment1.hpp
        src/assignment1/assignment1.cpp
)
target_include_directories(assignment1 PRIVATE src/assignment1)

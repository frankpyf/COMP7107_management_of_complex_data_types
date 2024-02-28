cmake_minimum_required(VERSION 3.25)

project(COMP7107
LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions("DATA_PATH=${CMAKE_SOURCE_DIR}/data")

add_subdirectory(src/assignment1)
add_subdirectory(src/assignment2)

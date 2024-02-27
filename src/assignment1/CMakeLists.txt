project(assignment1)

add_executable(assignment1)
target_sources(assignment1
    PRIVATE
        assignment1.hpp
        assignment1.cpp
)
target_include_directories(assignment1 PRIVATE assignment1)
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
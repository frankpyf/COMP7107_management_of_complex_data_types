project(COMP7107)

add_executable(assignment1)
target_sources(assignment1
    PRIVATE
        assignment1.hpp
        assignment1.cpp
)
target_include_directories(assignment1 PRIVATE assignment1)

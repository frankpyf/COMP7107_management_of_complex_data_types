project(COMP7107)

add_executable(assignment3_part1)
target_sources(assignment3_part1
    PRIVATE
        assignment3.hpp
        assignment3.part1.cpp
)


add_executable(assignment3_part2)
target_sources(assignment3_part2
    PRIVATE
        assignment3.hpp
        assignment3.part2.cpp
)
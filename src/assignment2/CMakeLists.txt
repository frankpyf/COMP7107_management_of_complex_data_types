project(COMP7107)

add_executable(assignment2_part1)
target_sources(assignment2_part1
    PRIVATE
        assignment2.hpp
        assignment2.part1.cpp
)
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(assignment2_part1 PRIVATE assignment2)

add_executable(assignment2_part2)
target_sources(assignment2_part2
    PRIVATE
        assignment2.hpp
        assignment2.part2.cpp
)
target_include_directories(assignment2_part2 PRIVATE assignment2)

add_executable(assignment2_part3)
target_sources(assignment2_part3
    PRIVATE
        assignment2.hpp
        assignment2.part3.cpp
)
target_include_directories(assignment2_part3 PRIVATE assignment2)
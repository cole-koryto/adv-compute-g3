cmake_minimum_required(VERSION 4.2)
project(homework3)

set(CMAKE_CXX_STANDARD 23)

add_executable(part1 part1.cpp trade.cpp)
add_executable(part2 part2.cpp trade.cpp)
add_executable(part3 part3.cpp trade.cpp)

add_compile_options(-fsanitize=address -g)                                       
add_link_options(-fsanitize=address)
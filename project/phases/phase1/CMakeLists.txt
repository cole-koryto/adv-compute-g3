cmake_minimum_required(VERSION 4.2)
project(phase1)

# add optimization flags for better performance
add_compile_options(-O3)

add_executable(main main.cpp phase1.cpp)

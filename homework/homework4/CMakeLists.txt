cmake_minimum_required(VERSION 3.10)
project(homework4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(homework4 src/main.cpp)
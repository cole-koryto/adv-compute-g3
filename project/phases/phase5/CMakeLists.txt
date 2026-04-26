cmake_minimum_required(VERSION 4.2)
project(phase5)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-O3)

add_executable(phase5_bench src/main.cpp src/order_book.cpp)
target_include_directories(phase5_bench PRIVATE include)

add_executable(phase5_test test/test_order_book.cpp src/order_book.cpp)
target_include_directories(phase5_test PRIVATE include)

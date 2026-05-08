cmake_minimum_required(VERSION 4.2)
project(homework5 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
include_directories(include)

add_executable(part1_hashtable src/part1_hashtable.cpp)
add_executable(part2_pqueue    src/part2_pqueue.cpp)
add_executable(part3_simd      src/part3_simd.cpp)
add_executable(part4_orderbook src/part4_orderbook.cpp)

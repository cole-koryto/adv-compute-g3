cmake_minimum_required(VERSION 3.16)
project(hftclient2026_kim LANGUAGES CXX)

add_executable(hftclient2026_kim main.cpp)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(hftclient2026_kim PRIVATE -O3 -Wall -Wextra -Wpedantic)
endif()

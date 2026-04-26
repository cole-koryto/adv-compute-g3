cmake_minimum_required(VERSION 4.2)
project(phase4)

set(CMAKE_CXX_STANDARD 23)

# Reasonable defaults for benchmarking; callers can override via CMAKE_CXX_FLAGS.
add_compile_options(-O3)

add_library(phase4_core
        src/MarketData.cpp
        src/MatchingEngine.cpp
        src/OrderManager.cpp
        src/TradeLogger.cpp
)

target_include_directories(phase4_core PUBLIC include)

add_executable(phase4_app src/main.cpp)
target_link_libraries(phase4_app PRIVATE phase4_core)

add_executable(phase4_test_latency test/test_latency.cpp)
target_link_libraries(phase4_test_latency PRIVATE phase4_core)


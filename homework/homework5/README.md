# Homework 5 — High-Performance Data Structures for Market Data


## Files

- `include/robin_hood_map.hpp` — Robin Hood open-addressed hash map (Part 1).
- `include/binary_heap.hpp` — array-backed binary heap (Part 2).
- `include/moving_average_simd.hpp` — scalar + AVX2 (via simde→NEON) moving average (Part 3).
- `include/order_book.hpp` — hybrid hash + ordered-map order book + naive baseline (Part 4).
- `include/simde/` — header-only AVX2→NEON shim for Apple Silicon.
- `src/partN_*.cpp` — benchmark driver for each part.
- `REPORT.md` — complexity, benchmarks, HFT discussion.

## Build

Requires CMake ≥ 4.2 and a C++23 compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Run

```bash
./build/part1_hashtable
./build/part2_pqueue
./build/part3_simd
./build/part4_orderbook
```

Each binary prints its own benchmark table to stdout.

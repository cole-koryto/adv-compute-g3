# Phase 5 — HFT Order Book

This folder contains a self-contained C++ project implementing the Phase 5 order book described in
`Phase 5_ HFT Order book.pdf`.

Core goals:
- String-based order IDs
- Fast add/modify/delete operations
- Benchmarking with `std::chrono`
- Unit tests + stress tests
- CSV output for plotting (Python script provided)

## Build

```bash
cd project/phases/phase5
mkdir -p build && cd build
cmake ..
cmake --build . -j
```

If you get `command not found: cmake`, install it first (macOS examples):

```bash
# Homebrew (recommended)
brew install cmake

# or conda
conda install -c conda-forge cmake
```

If you can’t install `cmake` right now, you can still build directly with `clang++`:

```bash
cd project/phases/phase5
clang++ -std=c++23 -O3 -Iinclude src/order_book.cpp src/main.cpp -o phase5_bench
clang++ -std=c++23 -O3 -Iinclude src/order_book.cpp test/test_order_book.cpp -o phase5_test
```

## Unit tests

```bash
./phase5_test
```

## Bench / stress test

Writes `phase5_bench.csv` in the current directory by default.

```bash
./phase5_bench --seed 1 --out phase5_bench.csv
```

## Plot

If you have matplotlib:

```bash
python3 ../scripts/plot.py phase5_bench.csv
```

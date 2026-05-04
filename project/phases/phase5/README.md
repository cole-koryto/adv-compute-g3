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

### Note: 

Expected Outputs:

```bash
N=1000 total=0.000799375s (add 0.000300625s, modify 0.000237917s, delete 0.000260833s)
N=5000 total=0.00531838s (add 0.00155575s, modify 0.00156788s, delete 0.00219475s)
N=10000 total=0.0107028s (add 0.00364725s, modify 0.00359713s, delete 0.00345842s)
N=50000 total=0.0485211s (add 0.0163503s, modify 0.0156857s, delete 0.0164851s)
N=100000 total=0.111731s (add 0.0316322s, modify 0.0397063s, delete 0.0403928s)
```

## Plot

If you have matplotlib:

```bash
python3 ../scripts/plot.py phase5_bench.csv
```

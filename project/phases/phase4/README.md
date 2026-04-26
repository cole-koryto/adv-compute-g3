# Phase 4 — Build & Benchmark a High-Frequency Trading System in C++

This folder contains a self-contained C++ project implementing the Phase 4 modules described in
`Phase 4_ Build & Benchmark a High-Frequency Trading System in C++.pdf`:

- Market data feed simulator (`MarketDataFeed`)
- Template-based limit order book (`OrderBook<Price, OrderId>`) using smart pointers + a memory resource
- Order management system (`OrderManager`) tracking order states (new/cancel/partial/filled)
- Matching engine (`MatchingEngine`) producing trades
- Trade logger (`TradeLogger`) with batched writes
- Tick-to-trade latency measurement + summary stats

## Build

```bash
cd project/phases/phase4
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
cd project/phases/phase4
clang++ -std=c++23 -O3 -Iinclude \
  src/MarketData.cpp src/OrderManager.cpp src/MatchingEngine.cpp src/TradeLogger.cpp src/main.cpp \
  -o phase4_app
clang++ -std=c++23 -O3 -Iinclude \
  src/MarketData.cpp src/OrderManager.cpp src/MatchingEngine.cpp src/TradeLogger.cpp test/test_latency.cpp \
  -o phase4_test_latency
```

## Run

```bash
./phase4_app --ticks 100000 --seed 1
./phase4_test_latency --ticks 10000 --seed 1
```

Both executables print tick-to-trade latency statistics (min/max/mean/stddev/p99) in nanoseconds.

# Phase 3

## Architecture

Three modules work together:

- **MarketSnapshot** — maintains a live view of bid/ask price levels using two `std::map<double, unique_ptr<PriceLevel>>` containers (bids sorted high→low, asks sorted low→high). Best bid/ask are always the map endpoints.
- **OrderManager** — tracks submitted orders in a `std::map<int, unique_ptr<MyOrder>>`. Assigns unique IDs, handles partial fills, and removes orders when fully filled or cancelled.
- **main.cpp** — loads a feed file, updates the snapshot on each event, places orders based on market conditions, and routes execution events back to the order manager.

## Memory Management

All heap objects (`PriceLevel`, `MyOrder`) are owned by `std::unique_ptr`. No raw `new` or `delete` anywhere. When a price level hits zero quantity or an order is fully filled/cancelled, the entry is erased from its map and the `unique_ptr` destructor automatically frees the memory.

## Build and Run

```bash
cd phases/phase3
mkdir build
cmake -B build
cmake --build build
```

The binary expects `sample_feed.txt` in the working directory where it is run. Run from the `phase3/` root:

```bash
./build/phase3
```

## Verify Memory Safety

```bash
leaks --atExit -- ./build/phase3
```

```
Process:         phase3 [68743]
Path:            /Users/USER/*/phase3
Load Address:    0x104a60000
Identifier:      phase3
Version:         0
Code Type:       ARM64
Platform:        macOS
Parent Process:  leaks [68742]
Target Type:     live task

Date/Time:       2026-04-19 14:15:03.456 -0400
Launch Time:     2026-04-19 14:15:03.326 -0400
OS Version:      macOS 26.2 (25C56)
Report Version:  7
Analysis Tool:   /Applications/Xcode.app/Contents/Developer/usr/bin/leaks
Analysis Tool Version:  Xcode 26.4 (17E192)

Physical footprint:         2512K
Physical footprint (peak):  2512K
Idle exit:                  untracked
----

leaks Report Version: 4.0, multi-line stacks
Process 68743: 192 nodes malloced for 17 KB
Process 68743: 0 leaks for 0 total leaked bytes.

```
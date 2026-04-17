# Phase 3 TODO

- [ ] `market_snapshot.h / .cpp` — class that maintains bid/ask levels, provides `get_best_bid()` and `get_best_ask()`, uses `std::unique_ptr` to manage each `PriceLevel`, removes levels when quantity reaches 0
- [ ] `order_manager.h / .cpp` — class that tracks placed orders, stores each `MyOrder` in a `std::unique_ptr`, supports: place, cancel, handle fill, print status
- [ ] `main.cpp` — strategy logic, loops over simulated feed, updates market snapshot and handles execution events, places orders in reaction to market data
- [ ] `sample_feed.txt` — simulated market feed to test the system
- [ ] `output.log` *(optional but encouraged)* — log of program's actions: price updates, order placements, fills
- [ ] `README.md` — overview of system architecture, explanation of how memory is managed safely, instructions on how to compile and run, how to verify correctness and memory safety

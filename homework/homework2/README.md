# README.md: Homework 2

This contains the screenshot output and the writeup for homework number 2 for advancec computing for finance.

## How to Run (homework/homework2)
```bash
cd homework/homework2
g++ -std=c++17 -O2 -o main main.cpp MarketData.cpp MarketDataFeed.cpp TradingEngine.cpp
./main
```

## Current Output for main.cpp


```bash
--- Performance Report ---
Total Market Ticks Processed: 100000
Total Orders Placed: 94077
Average Tick-to-Trade Latency (ns): 36113786
Maximum Tick-to-Trade Latency (ns): 64443375

--- Signal Contributions ---
Signal 1: 9911
Signal 2: 93328
Signal 3: 16652
Total Runtime (ms): 71
```
## Writeup
1. Which signal triggered the most orders? What could you optimize further? How would your code behave with 10x more data?

- Signal 2, with 93,328 triggers, had the most compared to signal 1 (the second highest) and signal 3 (the lowewst). 
- Looking into the code, this makes sense to us because Signal 2 checks for current price deviation from recent rolling averages by more than 2%. 
- So when we randomly generate prices between 100-200, Signal 2's check/condition is completed/satisfied frequently. And by placing 94,000 orders (approximately) with 100k ticks, our trading logic was used frequently on the tick data.
- Currently, removing the oldest element using erase() isn't efficient because a byproduct of htat would be the shifting of remaining elements every time to be called. If a deque (using std::deque) or some sort of buffer (I think circular would be okay personally) was used instead, the updates could remain efficient as data gets bigger.
- Running sum would be good as well to avoid recomputation of averages repeatedly. In terms of correctness our current version is okay, but scaling it would lag behind compared to this approach. The engine would be peforming more memory operations and signal checks, which would hurt performance. 

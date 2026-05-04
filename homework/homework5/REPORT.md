# Homework 5 Report

## Complexity Summary

| Structure | Insert | Lookup / Extract | Delete | Best-case query |
|---|---|---|---|---|
| Robin Hood hash table | O(1) avg | O(1) avg | O(1) avg | — |
| Binary heap | O(log N) | O(log N) pop | — | O(1) top |
| SIMD moving average | O(N·W) total, ~W/4 per output | — | — | — |
| Order book (hybrid) | O(log P) | O(1) by id | O(k) per level | O(1) BBO |
| Order book (naive) | O(1) | O(1) by id | O(1) | O(N) BBO |

## Part 1 — Robin Hood Hash Table

Open-addressed table with Robin Hood probing, power-of-2 capacity, 8-byte fixed keys, splitmix64 hash.

| Op (N = 2,097,152) | RobinHoodMap | `std::unordered_map` | Speedup |
|---|---:|---:|---:|
| Insert | 0.144 s | 0.291 s | 2.0× |
| Lookup | 0.124 s | 0.176 s | 1.4× |

Contiguous slots beat chained buckets once the working set exceeds L3 — fewer DRAM round-trips per probe. Trade-off: fixed key length, full-rehash resizes.

## Part 2 — Binary Heap

Array-backed max-heap, sift-up on push, swap-based sift-down on pop.

| Op (N = 2,097,152) | BinaryHeap | `std::priority_queue` |
|---|---:|---:|
| Insert all | 0.0463 s | 0.0434 s |
| Extract all | 0.8454 s | 0.3806 s |

Insert parity is expected. The 2.2× pop gap comes from `std::priority_queue` using hole-propagation (one move per level) and Floyd's sift-down (skips the parent compare); the textbook swap-based version does ~3× the writes.

## Part 3 — SIMD Moving Average

Parallel-windows layout: each SIMD lane accumulates a different output window. AVX2 intrinsics lowered to NEON via `simde` (Apple Silicon has no AVX2).

| Variant (N = 16.8M, W = 512) | Time | Speedup |
|---|---:|---:|
| scalar | 5.4689 s | 1.00× |
| simd_1 | 2.0444 s | 2.67× |
| simd_2 | 1.2159 s | 4.50× |
| simd_4 | 0.6762 s | 8.09× |

SIMD width gives ~4×; multiple accumulators break the fadd dependency chain for another ~2×. Numerical drift vs. scalar is < 2e-12.

## Part 4 — Order Book

Hybrid: `std::unordered_map<int, Order>` for id lookup + two `std::map<double, vector<int>>` price indexes (bids/asks). Naive baseline keeps only the hash map and linear-scans for BBO.

| Op (N = 524,288 inserts, M = 4,096 queries) | Hybrid | Naive |
|---|---:|---:|
| Insert all | 0.227 s | 0.014 s |
| BBO queries | 3.06e-5 s | 8.28 s |

Hybrid pays 16× on insert (extra rb-tree work per add) to make BBO ~270,000× faster (`map::begin()` is O(1) on the cached leftmost node vs. O(N) scan).

## HFT Relevance

BBO is read on every tick and quote; orders are added far less often. Paying 16× on the rare path to win 5 orders of magnitude on the hot path is the correct trade-off for a matching engine. Robin Hood hashing buys lower tail latency — what HFT cares about more than mean throughput. SIMD scaling shows that wide-vector + ILP can extract an order of magnitude on streaming numerical work like signal generation.

## Memory

Hybrid order book adds ~1–2 MB of rb-tree + per-level vector overhead on top of the ~25 MB hash map at 524k orders. Robin Hood slot is 16 B vs. ~32 B for `std::string`-keyed `unordered_map` nodes (plus per-node allocation). Heap is one contiguous `vector<Order>`.

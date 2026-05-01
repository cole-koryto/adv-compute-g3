# Homework 5 Report

## Part 1 — Robin Hood Hash Table

### Design

- **Open addressing + Robin Hood probing.** Slots stored in a contiguous `std::vector`; on collision, the candidate with the larger probe sequence length (PSL) keeps the slot, displaced entries continue probing. Compresses PSL variance, giving tight tail latency.
- **Sentinel PSL (`int32_t psl = -1`)** encodes empty slots without a separate flag. Slot size ends up at 16 B for `(array<char,8>, int)` after padding — 2× smaller than `std::string`-keyed alternatives (32 B) due to `std::string`'s 24 B size + 8 B alignment.
- **Fixed 8-byte keys** (`std::array<char, 8>`, zero-padded) enable a single-load custom hash: `memcpy` 8 bytes to `uint64_t`, run splitmix64 mixer (3 xor-shifts + 2 multiplies, ~10 instructions). Cited from Vigna's `splitmix64.c`.
- **Power-of-2 capacity** lets `bucket = hash & (capacity - 1)` replace modulo. ~20 cycles → 1 cycle per index calc.
- **Backward-shift erase** (no tombstones): pulls subsequent entries one slot back, decrementing PSL, until hitting `psl ≤ 0`. Preserves invariant; avoids load-factor decay over time.
- **Public API takes `std::string_view`** and normalizes to canonical 8-byte form internally — separates "transit type" (`string_view`) from "storage type" (`array<char,8>`).

### Benchmark (N = 2,097,152, pre-sized maps, sequential string keys)

| Operation | RobinHoodMap | `std::unordered_map` | Speedup |
|---|---:|---:|---:|
| Insert | 0.144 s | 0.291 s | **2.0×** |
| Lookup (hits) | 0.124 s | 0.176 s | **1.4×** |

### Analysis

At small N (~64K) where the working set fits in L2 cache, `std::unordered_map`'s mature optimization edges out our minimal implementation — pointer chasing is "free" in cache. At N ≥ 1M the working set exceeds L3; STL's chained-list layout pays full DRAM latency (~100 cycles) per pointer hop, while Robin Hood's contiguous slots stream through 1–2 cache lines per probe. The crossover near 10⁶ entries is exactly where cache effects flip the comparison.

**Trade-offs vs. chaining:**
- + Lower variance, predictable tail latency (HFT-relevant metric).
- + No per-node allocation; one buffer.
- + Backward-shift erase avoids tombstone-driven slowdown.
- − Fixed-size keys impose a length cap (8 chars; covers US tickers but not options symbols).
- − Resize is O(N) full rehash; partially hidden by amortization but still a cliff.
- − Pointer invalidation on resize; production code would expose `(index, generation)` handles.

**Future optimizations (not implemented):**
- Cache hash in slot to skip rehashing on resize and short-circuit key compares.
- SoA layout (separate PSL array) would unlock per-byte savings that AoS padding ate.
- Symbol interning to `uint32_t` IDs for unbounded length at fixed slot size.

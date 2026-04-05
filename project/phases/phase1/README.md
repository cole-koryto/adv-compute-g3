# Phase 1 - High-Performance Linear Algebra Kernels TODO COMPLETE BEFORE SUBMISSION

## Benchmarking and Profiling

Each function was benchmarked on square NxN matrices (N = 64, 128, 256, 512, 1024) over 20 runs per size. Timing used `std::chrono::high_resolution_clock` around the multiply call only (excluding allocation and data generation). Results report mean and standard deviation in milliseconds.

### With `-O3` optimization

| Size | Function | Avg (ms) | Stddev (ms) |
|------|----------|----------|-------------|
| 64 | mv_row_major | 0.0025 | 0.0005 |
| 64 | mv_col_major | 0.0043 | 0.0008 |
| 64 | mm_naive | 0.253 | 0.031 |
| 64 | mm_transposed_b | 0.156 | 0.019 |
| 128 | mv_row_major | 0.007 | 0.002 |
| 128 | mv_col_major | 0.016 | 0.007 |
| 128 | mm_naive | 1.783 | 0.568 |
| 128 | mm_transposed_b | 0.785 | 0.200 |
| 256 | mv_row_major | 0.021 | 0.006 |
| 256 | mv_col_major | 0.054 | 0.014 |
| 256 | mm_naive | 12.309 | 2.739 |
| 256 | mm_transposed_b | 5.028 | 0.431 |
| 512 | mv_row_major | 0.094 | 0.004 |
| 512 | mv_col_major | 0.290 | 0.013 |
| 512 | mm_naive | 128.023 | 4.719 |
| 512 | mm_transposed_b | 47.431 | 0.291 |
| 1024 | mv_row_major | 0.443 | 0.009 |
| 1024 | mv_col_major | 1.234 | 0.031 |
| 1024 | mm_naive | 1332.93 | 12.937 |
| 1024 | mm_transposed_b | 455.71 | 2.014 |

### Without optimization (`-O0`)

| Size | Function | Avg (ms) | Stddev (ms) |
|------|----------|----------|-------------|
| 64 | mv_row_major | 0.009 | 0.002 |
| 64 | mv_col_major | 0.009 | 0.003 |
| 64 | mm_naive | 0.734 | 0.179 |
| 64 | mm_transposed_b | 0.707 | 0.178 |
| 128 | mv_row_major | 0.022 | 0.009 |
| 128 | mv_col_major | 0.025 | 0.013 |
| 128 | mm_naive | 3.990 | 1.901 |
| 128 | mm_transposed_b | 3.419 | 1.156 |
| 256 | mv_row_major | 0.075 | 0.010 |
| 256 | mv_col_major | 0.168 | 0.008 |
| 256 | mm_naive | 41.091 | 0.343 |
| 256 | mm_transposed_b | 24.282 | 0.232 |
| 512 | mv_row_major | 0.295 | 0.007 |
| 512 | mv_col_major | 0.772 | 0.015 |
| 512 | mm_naive | 375.92 | 1.598 |
| 512 | mm_transposed_b | 198.42 | 1.374 |
| 1024 | mv_row_major | 1.191 | 0.108 |
| 1024 | mv_col_major | 3.214 | 0.085 |
| 1024 | mm_naive | 3200.74 | 31.656 |
| 1024 | mm_transposed_b | 1601.98 | 7.050 |

## Discussion Questions
### Explain the key differences between pointers and references in C++. When would you choose to use a pointer over a reference, and vice versa, in the context of implementing numerical algorithms?
### How does the row-major and column-major storage order of matrices affect memory access patterns and cache locality during matrix-vector and matrix-matrix multiplication? Provide specific examples from your implementations and benchmarking results.
### Describe how CPU caches work (L1, L2, L3) and explain the concepts of temporal and spatial locality. How did you try to exploit these concepts in your optimizations?
###  is memory alignment, and why is it important for performance? Did you observe a significant performance difference between aligned and unaligned memory in your experiments? Explain your findings.
### Discuss the role of compiler optimizations (like inlining) in achieving high performance. How did the optimization level affect the performance of your baseline and optimized implementations? What are the potential drawbacks of aggressive optimization?
### Based on your profiling experience, what were the main performance bottlenecks in your initial implementations? How did your profiling results guide your optimization efforts?
### Reflect on the teamwork aspect of this assignment. How did dividing the initial implementation tasks and then collaborating on analysis and optimization work? What were the challenges and benefits of this approach?
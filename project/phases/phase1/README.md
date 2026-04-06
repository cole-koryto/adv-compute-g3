# Phase 1 - High-Performance Linear Algebra Kernels 

## Build and Run

```bash
cd project/phases/phase1
mkdir -p build && cd build
cmake .. -DCMAKE_CXX_FLAGS="-O3 -g"
make
```

The `-O3` flag enables compiler optimizations and `-g` adds debug symbols for profiling with Instruments.

Run modes:

```bash
./main              # correctness tests (default)
./main bench        # benchmarking (baseline timing)
./main bench-align  # memory alignment benchmark
./main profile      # profiling (runs mm_naive and mm_transposed_b 10x on 1024x1024)
```

## Benchmarking and Profiling

Each function was benchmarked on square NxN matrices (N = 64, 128, 256, 512, 1024) over 20 runs per size. Timing used `std::chrono::high_resolution_clock` around the multiply call only (excluding allocation and data generation). Results report mean and standard deviation in milliseconds.

### With `-O3` optimization

| Size | Function | Avg (ms) | Stddev (ms) |
|------|----------|----------|-------------|
| 64 | mv_row_major | 0.0020334 | 0.000871734 |
| 64 | mv_col_major | 0.00443545 | 0.00181468 |
| 64 | mm_naive | 0.351846 | 0.438274 |
| 64 | mm_transposed_b | 0.126742 | 0.0730503 |
| 64 | mm_optimized | 0.0693792 | 0.0186308 |
| 128 | mv_row_major | 0.0056562 | 9.21638e-05 |
| 128 | mv_col_major | 0.0121938 | 0.000110614 |
| 128 | mm_naive | 1.59885 | 0.00999228 |
| 128 | mm_transposed_b | 0.720085 | 0.00271691 |
| 128 | mm_optimized | 0.405856 | 0.00347975 |
| 256 | mv_row_major | 0.0305876 | 0.000825161 |
| 256 | mv_col_major | 0.0695124 | 0.00335166 |
| 256 | mm_naive | 18.3181 | 0.126508 |
| 256 | mm_transposed_b | 9.17798 | 0.0392309 |
| 256 | mm_optimized | 3.67675 | 0.024557 |
| 512 | mv_row_major | 0.166764 | 0.0631289 |
| 512 | mv_col_major | 0.419242 | 0.254654 |
| 512 | mm_naive | 184.089 | 12.5605 |
| 512 | mm_transposed_b | 77.3535 | 1.23002 |
| 512 | mm_optimized | 34.7869 | 2.64299 |
| 1024 | mv_row_major | 0.720327 | 0.00244212 |
| 1024 | mv_col_major | 1.44778 | 0.0321276 |
| 1024 | mm_naive | 2035.49 | 9.80046 |
| 1024 | mm_transposed_b | 740.791 | 35.2292 |
| 1024 | mm_optimized | 309.573 | 0.352854 |

### Without optimization (`-O0`)

| Size | Function | Avg (ms) | Stddev (ms) |
|------|----------|----------|-------------|
| 64 | mv_row_major | 0.0092209 | 0.00367494 |
| 64 | mv_col_major | 0.00818535 | 0.00318406 |
| 64 | mm_naive | 0.629352 | 0.1804 |
| 64 | mm_transposed_b | 0.65496 | 0.163748 |
| 64 | mm_optimized | 0.496152 | 0.127236 |
| 128 | mv_row_major | 0.0333105 | 0.0119082 |
| 128 | mv_col_major | 0.0390626 | 0.00958077 |
| 128 | mm_naive | 6.32702 | 0.269928 |
| 128 | mm_transposed_b | 4.69785 | 0.0721536 |
| 128 | mm_optimized | 3.6204 | 0.0317699 |
| 256 | mv_row_major | 0.136254 | 0.0571128 |
| 256 | mv_col_major | 0.257331 | 0.0101137 |
| 256 | mm_naive | 68.0564 | 0.657049 |
| 256 | mm_transposed_b | 37.9747 | 0.197806 |
| 256 | mm_optimized | 28.9425 | 0.111139 |
| 512 | mv_row_major | 0.523552 | 0.171493 |
| 512 | mv_col_major | 1.14502 | 0.0223036 |
| 512 | mm_naive | 602.7 | 20.125 |
| 512 | mm_transposed_b | 312.735 | 0.888471 |
| 512 | mm_optimized | 235.64 | 0.849742 |
| 1024 | mv_row_major | 1.91212 | 0.632103 |
| 1024 | mv_col_major | 4.70922 | 0.0337035 |
| 1024 | mm_naive | 4937.19 | 81.0739 |
| 1024 | mm_transposed_b | 2445.85 | 13.0341 |
| 1024 | mm_optimized | 1908.61 | 4.06285 |

## Discussion Questions
### Explain the key differences between pointers and references in C++. When would you choose to use a pointer over a reference, and vice versa, in the context of implementing numerical algorithms?
- References are defined typically with type&, and aliases an existing variable. Once references are established/binded to a valid object/variable (which it must do), they can't be null or reseated.References are for simpler and safer tasks.
- Pointers are defined typically with type*, and have a memory address. This memory address can be reassigned or set to a null pointer, but is useful for memory allocation tasks and arithmetic with pointers as well. For overal low-evel, high performance code, they're favored.
- Overall, for numerical algorithms, references might be used for scalar parameters, while pointers could be done for simple indexing and manual memory allocation. Pointers are also used for representing contiguous blocks of memory well in a dynamic fashion


### How does the row-major and column-major storage order of matrices affect memory access patterns and cache locality during matrix-vector and matrix-matrix multiplication? Provide specific examples from your implementations and benchmarking results.
- First, some definitions
  - Row-major: stores rows contiguously
  - column-major: stores columns contiguously
- If you do Matrix Vector Multiplication using:
  - Row-major: this is good spatial locality, clearly showing that it was faster than column-major in terms of runtime. There's not a lot of cache misses and memory is accessed sequentially.
  - Coumn-major: a lot of cache misses here and stride access may be a cause of it. Larger strides are definitely the issue here based on our computation.
- Matrix Matrix Multiplication is similar; the naive iplementation has poor cache locality, but when we have our transposed and optimized versions, performance is improved because contiguous memory access is defined well, whether through blocks or row-major usage.

### Describe how CPU caches work (L1, L2, L3) and explain the concepts of temporal and spatial locality. How did you try to exploit these concepts in your optimizations?

- Modern CPUs use a hierarchy of caches (L1, L2, and L3) to reduce memory access latency. From one of the quizzes we took before this, L1 cache is the smallest and fastest, while L3 is larger but slower. 
- Two key concepts that influence performance are spatial and temporal locality. 
  - Spatial locality refers to accessing memory locations that are close together, while temporal locality refers to reusing recently accessed data. 
  - Blocking, for example, is an example of temporal locality, where in our optimization method we kept A/B blocks as well as the C block in cache. Blocking improves reuse while lowering time when we did optimize them.
- In our optimized implementation, we improved spatial locality by accessing matrix B and the result matrix in contiguous memory order within the innermost loop. 
  - We also improved temporal locality by reusing elements of matrix A (A[i,k]) multiple times before moving on. 
  - We did these changes to reduce cache misses and improve overall performance, which is reflected in the significant speedup of the optimized implementation compared to the naive version.

###  is memory alignment, and why is it important for performance? Did you observe a significant performance difference between aligned and unaligned memory in your experiments? Explain your findings.
- Memory alignment ensures that data is stored at memory addresses that are multiples of a specific boundary (e.g., 64 bytes), which matches the size of cache lines and SIMD instructions. Proper alignment can improve performance by enabling more efficient memory access and vectorized operations. 
- However, in our code, we observed little to no performance improvement when using aligned memory compared to unaligned memory. 
  - This is likely because our implementation does not explicitly use SIMD instructions and is more limited by memory access patterns than alignment. 
- So while alignment is certainly important in highly optimized numerical libraries, its overall impact in our implementation seemed to be minimal compared to other optimizations such as improving cache locality, which we described above
### Discuss the role of compiler optimizations (like inlining) in achieving high performance. How did the optimization level affect the performance of your baseline and optimized implementations? What are the potential drawbacks of aggressive optimization?
- Compiler optimization levels have a significant impact on performance. When comparing -O0 (no optimization) to -O3 (aggressive optimization), we observed substantial speed improvements across all implementations, particularly in matrix-matrix multiplication. 
- The -O3 optimization level enables techniques such as loop unrolling, vectorization, and inlining, which reduce overhead and improve instruction-level efficiency. 
  - Inlining eliminates function call overhead for small functions, which can be beneficial in performance-critical code. 
- However, aggressive optimization can increase binary size and make debugging more difficult. Overall, our results show that compiler optimizations play a major role in achieving high performance, often providing large gains with minimal changes to the source code.

### Based on your profiling experience, what were the main performance bottlenecks in your initial implementations? How did your profiling results guide your optimization efforts?
- Profiling revealed that the majority of execution time is spent inside the innermost loops of the matrix-matrix multiplication functions. 
  - This was expected by us, as these loops perform the bulk of the computation and have cubic time complexity. 
  - The naive implementation suffers from inefficient memory access patterns, particularly when accessing matrix B, leading to frequent cache misses. 
- Profiling results guided our optimization efforts by highlighting the importance of improving memory access patterns. 
  - By reordering loops and using techniques such as transposing matrix B or restructuring computation, we were able to significantly reduce runtime. This demonstrates that performance bottlenecks in numerical algorithms are often driven more by memory access than by arithmetic operations.


### Reflect on the teamwork aspect of this assignment. How did dividing the initial implementation tasks and then collaborating on analysis and optimization work? What were the challenges and benefits of this approach?
- We divided the assignment among members, where each person was able to focus on a specific function/aspect to ensure correctness.
  - We think this approach made the process more efficient and helped build a strong understanding of each algorithm.
- Afterwards, we tried to collaborate on performance analysis as well tot alk about our insights. 
- Probably the most difficult aspect was that we had to remain consistency in our approaches and as tasks were passed on, it took a bit longer for the next individual to see what was being done
  - But we think because this was an overall simpler assignment, these problems were minimized, but working concurrently could be something we try in the future.

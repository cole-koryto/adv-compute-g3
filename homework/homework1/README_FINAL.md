# Homework 1: Fast Matrix Summation - Conclusions

## Overview

This assignment focused on improving the performance of summing every element in a large `SIZE x SIZE` integer matrix. The provided starter implementation used two helper functions, `getElement()` and `add()`, inside the inner loop of the summation routine. My final implementation keeps the same row-major traversal order but removes those helper-function calls and performs the addition directly with:

```cpp
sum += matrix[i][j];
```

The optimized version produces the same sum as the basic version during each individual run while reducing execution time.

## Implementation Summary

The file [homework1.cpp](/mnt/c/Users/colek/CLionProjects/adv-compute-g3/homework/homework1/homework1.cpp) contains two summation functions:

- `sumMatrixBasic()`: the original baseline implementation that calls `add(sum, getElement(matrix, i, j))` for every matrix element.
- `sumMatrixOptimized()`: the final implementation that accesses the element directly and adds it to the running total without helper-function calls.

The final optimized function is:

```cpp
long long sumMatrixOptimized(const std::vector<std::vector<int>>& matrix) {
    long long sum = 0;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            sum += matrix[i][j];
    return sum;
}
```

## Why the Basic Implementation Is Slower

The baseline code is correct, but it adds avoidable overhead inside the hottest part of the program: the inner loop that runs once per matrix element.

### 1. Function call overhead

For every element, the basic version calls:

- `getElement(matrix, i, j)`
- `add(sum, value)`

That means two extra function calls are introduced for each access and accumulation. Since the matrix contains `4096 x 4096` elements, this overhead is repeated a very large number of times. Even if the compiler can optimize some of this away, writing the loop directly gives it a simpler and more optimization-friendly structure.

### 2. Extra abstraction in the inner loop

The optimized version removes the helper calls and keeps only the direct indexed access and accumulation. This reduces work per iteration and makes the core loop easier for the compiler to optimize.

### 3. Cache locality

Both versions iterate row by row:

```cpp
for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
        ...
    }
}
```

This is a good access pattern for `std::vector<std::vector<int>>` because each inner vector stores its row contiguously. Traversing `j` from left to right uses contiguous memory within each row, which supports good cache behavior. As a result, the main improvement in this submission comes from reducing loop overhead rather than changing traversal order.

## Optimization Attempts Considered

In addition to the final version, I considered and tested other approaches.

### Pointer arithmetic

One attempted form was:

```cpp
*((*(matrix.data() + i)).data() + j);
```

This approach did improve on the basic version, but it was not faster than the final direct-access implementation. Based on the code structure, a reasonable explanation is that it introduced extra `.data()` operations and produced code that was less straightforward than plain indexing. In this case, lower-level pointer syntax did not translate into better practical performance.

### Single-loop indexing

Another attempted form was:

```cpp
sum += matrix[i / SIZE][i % SIZE];
```

This also improved over the basic version but did not outperform the final nested-loop version. A likely reason is that replacing two loops with one loop required division and modulus for every element access, and those extra arithmetic operations offset the benefit of collapsing the loops.

## Measured Performance

The following performance measurements were recorded utilizing the final code version discussed.

### Default compilation

Command:

```bash
g++ homework/homework1/homework1.cpp
```

Results:

- Basic Time: `112 ms`
- Optimized Time: `76 ms`

This is a reduction of approximately 32%.

### `-O3`

Command:

```bash
g++ homework/homework1/homework1.cpp -O3
```

Results:

- Basic Time: `6 ms`
- Optimized Time: `3 ms`

This is a reduction of 50%.

### `-O3 -march=native -flto -funroll-loops`

Command:

```bash
g++ homework/homework1/homework1.cpp -O3 -march=native -flto -funroll-loops
```

Results:

- Basic Time: `4 ms`
- Optimized Time: `2 ms`

This is also a reduction of 50%.

## Interpreting the Results

These measurements support several conclusions.

### 1. The optimized implementation is consistently faster

Across all recorded compiler settings, the direct-access version outperformed the baseline implementation. That supports the conclusion that simplifying the inner loop was a meaningful optimization.

### 2. Compiler optimization flags had a major effect

The largest absolute performance gains came from compiler optimization settings, especially `-O3` and the more aggressive flag combination `-O3 -march=native -flto -funroll-loops`. This suggests that the compiler was able to apply powerful low-level optimizations once the loop body was written in a simple form.

### 3. Simpler code outperformed the more manual alternatives

Although pointer arithmetic and single-loop indexing were explored, the plain nested-loop version with direct element access performed best in the recorded tests. This is an important result: lower-level code is not automatically faster. In this case, the clearest implementation also appears to have been the most effective.

## Target Performance

The assignment asks for a target execution time based on the test environment. The original notes do not record one formal target value in advance, so this report does not claim a specific preselected threshold that was proven beforehand.

However, the measured results show that the optimized version:

- ran in `76 ms` with default compilation,
- ran in `3 ms` with `-O3`,
- ran in `2 ms` with `-O3 -march=native -flto -funroll-loops`.

Based on those measurements, the final implementation clearly achieved a significant speedup over the baseline on the reference system used for testing.

## Correctness

Correctness was verified by comparing the output of the basic and optimized implementations during each run. In every recorded case, both functions produced the same total for the generated matrix. Because the matrix contents are randomized at runtime, matching sums within the same execution is the correct validation criterion.

## Final Conclusion

The most effective optimization in this assignment was simplifying the summation loop so that each element is accessed and accumulated directly:

```cpp
sum += matrix[i][j];
```

This removed unnecessary helper-function overhead while preserving the cache-friendly row-major traversal pattern already present in the baseline code. Testing showed that this change consistently improved performance, and that compiler optimization flags amplified the improvement substantially.

The results also demonstrate an important systems-programming lesson: the best-performing solution is not always the one that looks the most low-level. In this case, direct indexed access inside a straightforward nested loop performed better than more manual pointer-based or arithmetic-heavy alternatives.

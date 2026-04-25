# HFT Template Programming Homework 4

This project implements a header-only, policy-based order engine demo using advanced C++ templates, compile-time logic, generic algorithms, and configurable allocation/threading policies.

## Completed Assignment Features

- Template metaprogramming utilities for `Factorial`, `Fibonacci`, `IsOdd`, `print_if_odd`, and variadic `print_all`.
- Compile-time validation with `static_assert`, `std::enable_if_t`, and `std::void_t`.
- `constexpr` math helpers for factorial, Fibonacci, square, and HFT-style price bucket rounding.
- A fixed-size `StaticVector<T, N>` container with `push_back`, `operator[]`, `size`, `begin`, and `end`.
- A generic custom `find_if` algorithm used with lambda predicates over order data.
- A policy-based `OrderBookBuffer<T, AllocatorPolicy, ThreadingPolicy>` supporting stack and heap allocation.
- Threading policies for `NoLock` and `MutexLock`.
- Optional `ZeroInitAllocator` policy for zero-initialized heap allocation.
- Demo coverage in `src/main.cpp` for compile-time checks, runtime checks, static vector searches, and two differently configured order buffers.

## Project Structure

```text
homework4/
├── CMakeLists.txt
├── include/
│   ├── metaprogramming.hpp
│   ├── constexpr_math.hpp
│   ├── StaticVector.hpp
│   ├── generic_algorithms.hpp
│   └── OrderBookBuffer.hpp
├── src/
│   └── main.cpp
└── README.md
```

## File Summary

- `include/metaprogramming.hpp`: TMP factorial/Fibonacci, odd-value trait, SFINAE-enabled `print_if_odd`, and variadic `print_all`.
- `include/constexpr_math.hpp`: `constexpr` `price_bucket`, `square`, factorial, Fibonacci, and a compile-time sized array example.
- `include/StaticVector.hpp`: Fixed-capacity stack-backed vector-like container with iterator support.
- `include/generic_algorithms.hpp`: Custom iterator-based `find_if`.
- `include/OrderBookBuffer.hpp`: Allocator policies, threading policies, RAII lock guard, and configurable order buffer.
- `src/main.cpp`: End-to-end demonstration of every homework section.

## Build And Run

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
./build/homework4
```

## Example Output

```text
HFT Template Homework Starter
5
7, 8, 1,
Price > 100: 100.5, qty: 10
Price > 100: 120, qty: 5
First qty divisible by 10: 10
Book 1: StackAllocator + NoLock
Order{price=99.5, quantity=100}
Order{price=101.25, quantity=50}

Book 2: HeapAllocator + MutexLock
Order{price=102.75, quantity=200}
Order{price=98.1, quantity=75}
```

## Notes

- The project currently builds with C++20 in `CMakeLists.txt`.
- The main executable target is named `homework4`.
- `StackAllocator<Order, 10>` is used for the no-lock order book, and `HeapAllocator<Order>` is used for the mutex-protected order book.

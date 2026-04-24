#pragma once
#include <type_traits>

// TMP-based factorial
template<int N> struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};
template<> struct Factorial<0> {
    static constexpr int value = 1;
};

// TMP-based fibonacci
template<int N> struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};
template<> struct Fibonacci<1> {
    static constexpr int value = 1;
};
template<> struct Fibonacci<0> {
    static constexpr int value = 0;
};

// TMP-based odd check and print
template<int N> struct IsOdd {
    static constexpr bool value = (N % 2 != 0);
};
template<int N>
std::enable_if_t<IsOdd<N>::value> print_if_odd()
{
    std::cout << N << std::endl;
};

// TMP-based print all
template<typename... Args>
void print_all(Args... args)
{
    ((std::cout << args << ", "), ...) << std::endl;  // fold expression
};
#pragma once
#include <iostream>
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
template<typename T, typename = void>
struct IsOdd : std::false_type {};

template<typename T>
struct IsOdd<T, std::void_t<decltype(T::value)>>
    : std::bool_constant<
          std::is_integral_v<decltype(T::value)> && (T::value % 2 != 0)> {};

template<typename T, std::enable_if_t<IsOdd<T>::value, int> = 0>
void print_if_odd(T)
{
    std::cout << T::value << std::endl;
}

// TMP-based print all
template<typename... Args>
void print_all(Args... args)
{
    ((std::cout << args << ", "), ...) << std::endl;  // fold expression
}

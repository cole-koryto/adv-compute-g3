#pragma once
#include <type_traits>

// TMP-based factorial
template<int N> struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};
template<> struct Factorial<0> {
    static constexpr int value = 1;
};

// Add more TMP utilities, traits, and enable_if functions here
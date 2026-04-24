#include <iostream>
#include "metaprogramming.hpp"
#include "constexpr_math.hpp"
#include "StaticVector.hpp"
#include "generic_algorithms.hpp"
#include "OrderBookBuffer.hpp"

int main() {
    std::cout << "HFT Template Homework Starter\n";

    // Tests metaprogramming
    static_assert(Factorial<5>::value == 120, "Incorrect Factorial");
    static_assert(Fibonacci<7>::value == 13, "Incorrect Fibonacci");
    static_assert(IsOdd<1>::value, "Incorrect IsOdd");
    print_if_odd<5>();
    print_all(7, 8, 1);


    return 0;
}
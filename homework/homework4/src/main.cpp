#include <iostream>
#include <cassert>
#include "metaprogramming.hpp"
#include "constexpr_math.hpp"
#include "StaticVector.hpp"
#include "generic_algorithms.hpp"
#include "OrderBookBuffer.hpp"

struct Order {
    double price;
    int quantity;
};

int main() {
    std::cout << "HFT Template Homework Starter\n";

    // Tests metaprogramming
    static_assert(Factorial<5>::value == 120, "Incorrect Factorial");
    static_assert(Fibonacci<7>::value == 13, "Incorrect Fibonacci");
    static_assert(IsOdd<std::integral_constant<int, 1>>::value, "Incorrect IsOdd");
    print_if_odd(std::integral_constant<int, 5>{});
    print_all(7, 8, 1);

    // Tests constexpr_math
    static_assert(price_bucket(17.07) == 17.05, "Incorrect price_bucket");
    static_assert(price_bucket(101.73) == 101.70, "Incorrect price_bucket");
    static_assert(factorial(5) == 120, "Incorrect factorial");
    static_assert(fibonacci(7)== 13, "Incorrect fibonacci");
    assert(price_bucket(17.07) == 17.05);
    assert(price_bucket(101.73) == 101.70);
    assert(factorial(5) == 120);
    assert(fibonacci(7) == 13);

    // Tests StaticVector
    StaticVector<Order, 4> orders;
    orders.push_back({100.5, 10});
    orders.push_back({120.0, 5});
    orders.push_back({90.0, 20});

    // Find all orders with price > 100
    auto it = orders.begin();
    while (it != orders.end()) {
        it = find_if(it, orders.end(), [](const Order& o) {
            return o.price > 100;
        });

        if (it != orders.end()) {
            std::cout << "Price > 100: " << it->price
                      << ", qty: " << it->quantity << std::endl;
            ++it;  // move forward to avoid infinite loop
        }
    }

    //Find the first order with quantity divisible by 10
    it = find_if(orders.begin(), orders.end(), [](const Order& o) {
    return o.quantity % 10 == 0;
    });

    if (it != orders.end()) {
        std::cout << "First qty divisible by 10: "
                  << it->quantity << std::endl;
    } else {
        std::cout << "No match found\n";
    }

    return 0;
}

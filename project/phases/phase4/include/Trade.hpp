#pragma once

#include <chrono>
#include <string>
#include <type_traits>

namespace hft::phase4
{
template <typename PriceType, typename OrderIdType>
struct Trade
{
    static_assert(std::is_integral_v<OrderIdType>, "Order ID must be an integer");
    static_assert(std::is_arithmetic_v<PriceType>, "Price type must be arithmetic");

    std::string symbol;
    OrderIdType buy_order_id{};
    OrderIdType sell_order_id{};
    PriceType price{};
    int quantity = 0;
    std::chrono::high_resolution_clock::time_point timestamp{};
};
} // namespace hft::phase4


#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

namespace hft::phase4
{
enum class Side : std::uint8_t
{
    Buy,
    Sell
};

enum class OrderStatus : std::uint8_t
{
    New,
    Cancelled,
    PartiallyFilled,
    Filled
};

template <typename PriceType, typename OrderIdType>
struct Order
{
    static_assert(std::is_integral_v<OrderIdType>, "Order ID must be an integer");
    static_assert(std::is_arithmetic_v<PriceType>, "Price type must be arithmetic");

    OrderIdType id{};
    std::string symbol;
    Side side = Side::Buy;
    PriceType price{};
    int quantity = 0;
    int filled = 0;
    OrderStatus status = OrderStatus::New;

    Order(OrderIdType order_id, std::string sym, Side s, PriceType p, int qty)
        : id(order_id), symbol(std::move(sym)), side(s), price(p), quantity(qty)
    {
    }

    int remaining() const noexcept { return quantity - filled; }
};
} // namespace hft::phase4


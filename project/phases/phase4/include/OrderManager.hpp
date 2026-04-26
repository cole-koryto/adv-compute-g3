#pragma once

#include "Order.hpp"

#include <cstdint>
#include <type_traits>
#include <unordered_map>

namespace hft::phase4
{
template <typename OrderIdType>
class OrderManager
{
public:
    static_assert(std::is_integral_v<OrderIdType>, "Order ID must be an integer");

    struct State
    {
        OrderStatus status = OrderStatus::New;
        int original_qty = 0;
        int filled_qty = 0;
    };

    void on_new(OrderIdType id, int quantity);
    void on_cancel(OrderIdType id);
    void on_fill(OrderIdType id, int fill_qty);

    const State *get(OrderIdType id) const;
    std::size_t active_count() const { return states_.size(); }

private:
    std::unordered_map<OrderIdType, State> states_;
};
} // namespace hft::phase4


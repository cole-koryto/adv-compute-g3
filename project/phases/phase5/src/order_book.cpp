#include "../include/order_book.hpp"

namespace hft::phase5
{
void OrderBook::add_order(const std::string &id, double price, int quantity, bool is_buy)
{
    Order order{id, price, quantity, is_buy};

    auto &levels = is_buy ? bids_ : asks_;
    levels[price][id] = order;
    order_lookup_[id] = std::move(order);
}

bool OrderBook::modify_order(const std::string &id, double new_price, int new_quantity)
{
    const auto it = order_lookup_.find(id);
    if (it == order_lookup_.end())
        return false;

    const Order old = it->second;
    auto &old_levels = old.is_buy ? bids_ : asks_;
    const auto level_it = old_levels.find(old.price);
    if (level_it != old_levels.end())
    {
        level_it->second.erase(id);
        if (level_it->second.empty())
            old_levels.erase(level_it);
    }

    add_order(id, new_price, new_quantity, old.is_buy);
    return true;
}

bool OrderBook::delete_order(const std::string &id)
{
    const auto it = order_lookup_.find(id);
    if (it == order_lookup_.end())
        return false;

    const Order old = it->second;
    auto &levels = old.is_buy ? bids_ : asks_;
    const auto level_it = levels.find(old.price);
    if (level_it != levels.end())
    {
        level_it->second.erase(id);
        if (level_it->second.empty())
            levels.erase(level_it);
    }

    order_lookup_.erase(it);
    return true;
}

const Order *OrderBook::find(const std::string &id) const
{
    const auto it = order_lookup_.find(id);
    if (it == order_lookup_.end())
        return nullptr;
    return &it->second;
}
} // namespace hft::phase5


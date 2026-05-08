#pragma once

#include <map>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace hft::phase5
{
struct Order
{
    std::string id; // string-based order ID
    double price = 0.0;
    int quantity = 0;
    bool is_buy = true;
};

class OrderBook
{
public:
    using Level = std::unordered_map<std::string, Order>;
    using Levels = std::map<double, Level>;

    void reserve(std::size_t n) { order_lookup_.reserve(n); }

    void add_order(const std::string &id, double price, int quantity, bool is_buy);
    bool modify_order(const std::string &id, double new_price, int new_quantity);
    bool delete_order(const std::string &id);

    const Order *find(const std::string &id) const;
    std::size_t size() const noexcept { return order_lookup_.size(); }

private:
    Levels bids_;
    Levels asks_;
    std::unordered_map<std::string, Order> order_lookup_;
};
} // namespace hft::phase5


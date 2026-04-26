#pragma once

#include "Memory.hpp"
#include "Order.hpp"

#include <map>
#include <memory_resource>

namespace hft::phase4
{
template <typename PriceType, typename OrderIdType>
class OrderBook
{
public:
    using OrderT = Order<PriceType, OrderIdType>;
    using OrderPtr = std::unique_ptr<OrderT, PmrDeleter<OrderT>>;

    using BuyBook = std::pmr::multimap<PriceType, OrderPtr, std::greater<PriceType>>;
    using SellBook = std::pmr::multimap<PriceType, OrderPtr, std::less<PriceType>>;

    explicit OrderBook(std::pmr::memory_resource *resource = std::pmr::get_default_resource())
        : resource_(resource), buys_(resource), sells_(resource)
    {
    }

    OrderPtr make_order(OrderIdType id, std::string symbol, Side side, PriceType price, int quantity)
    {
        return pmr_make_unique<OrderT>(resource_, id, std::move(symbol), side, price, quantity);
    }

    void add(OrderPtr order)
    {
        if (!order)
            return;
        const auto price = order->price;
        if (order->side == Side::Buy)
            buys_.emplace(price, std::move(order));
        else
            sells_.emplace(price, std::move(order));
    }

    const OrderT *best_bid() const
    {
        if (buys_.empty())
            return nullptr;
        return buys_.begin()->second.get();
    }

    const OrderT *best_ask() const
    {
        if (sells_.empty())
            return nullptr;
        return sells_.begin()->second.get();
    }

    BuyBook &buys() { return buys_; }
    SellBook &sells() { return sells_; }
    const BuyBook &buys() const { return buys_; }
    const SellBook &sells() const { return sells_; }

private:
    std::pmr::memory_resource *resource_ = nullptr;
    BuyBook buys_;
    SellBook sells_;
};
} // namespace hft::phase4


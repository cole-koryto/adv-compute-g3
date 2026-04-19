#include "market_snapshot.h"

void MarketSnapshot::update_bid(double price, int qty)
{
    if (qty == 0)
    {
        bids.erase(price);
    }
    else
    {
        bids[price] = std::make_unique<PriceLevel>(price, qty);
    }
}

void MarketSnapshot::update_ask(double price, int qty)
{
    if (qty == 0)
    {
        asks.erase(price);
    }
    else
    {
        asks[price] = std::make_unique<PriceLevel>(price, qty);
    }
}

// std::map is a red-black tree, so begin() gives leftmost (lowest) and rbegin() gives rightmost (highest)
// red black tree is a self-balancing binary search tree, so it maintains sorted order of keys (prices in this case)

const PriceLevel *MarketSnapshot::get_best_bid() const
{
    if (bids.empty())
        return nullptr;
    return bids.rbegin()->second.get(); // highest price
}

const PriceLevel *MarketSnapshot::get_best_ask() const
{
    if (asks.empty())
        return nullptr;
    return asks.begin()->second.get(); // lowest price
}
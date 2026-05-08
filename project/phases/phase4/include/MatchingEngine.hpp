#pragma once

#include "OrderBook.hpp"
#include "OrderManager.hpp"
#include "Trade.hpp"

#include <chrono>
#include <vector>

namespace hft::phase4
{
template <typename PriceType, typename OrderIdType>
class MatchingEngine
{
public:
    using OrderBookT = OrderBook<PriceType, OrderIdType>;
    using OrderT = typename OrderBookT::OrderT;
    using OrderPtr = typename OrderBookT::OrderPtr;
    using TradeT = Trade<PriceType, OrderIdType>;

    MatchingEngine(OrderBookT &book, OrderManager<OrderIdType> &oms) : book_(book), oms_(oms) {}

    std::vector<TradeT> submit(OrderPtr incoming, std::chrono::high_resolution_clock::time_point ts);

private:
    OrderBookT &book_;
    OrderManager<OrderIdType> &oms_;
};
} // namespace hft::phase4


#include "../include/MatchingEngine.hpp"

#include <algorithm>

namespace hft::phase4
{
template <typename PriceType, typename OrderIdType>
std::vector<typename MatchingEngine<PriceType, OrderIdType>::TradeT>
MatchingEngine<PriceType, OrderIdType>::submit(OrderPtr incoming, std::chrono::high_resolution_clock::time_point ts)
{
    std::vector<TradeT> trades;
    if (!incoming)
        return trades;

    const auto symbol = incoming->symbol;
    const auto incoming_side = incoming->side;

    const auto match_against = [&](auto &opposite) {
        while (incoming->remaining() > 0 && !opposite.empty())
        {
            auto it = opposite.begin(); // best ask or best bid (buys stored as greater<> so begin() is best bid)
            const PriceType resting_price = it->first;

            const bool crosses = (incoming_side == Side::Buy) ? (incoming->price >= resting_price)
                                                             : (incoming->price <= resting_price);
            if (!crosses)
                break;

            auto &resting_ptr = it->second;
            auto *resting = resting_ptr.get();

            const int fill_qty = std::min(incoming->remaining(), resting->remaining());

            incoming->filled += fill_qty;
            resting->filled += fill_qty;

            oms_.on_fill(incoming->id, fill_qty);
            oms_.on_fill(resting->id, fill_qty);

            TradeT trade;
            trade.symbol = symbol;
            trade.price = resting_price;
            trade.quantity = fill_qty;
            trade.timestamp = ts;

            if (incoming_side == Side::Buy)
            {
                trade.buy_order_id = incoming->id;
                trade.sell_order_id = resting->id;
            }
            else
            {
                trade.buy_order_id = resting->id;
                trade.sell_order_id = incoming->id;
            }
            trades.push_back(std::move(trade));

            if (resting->remaining() == 0)
            {
                resting->status = OrderStatus::Filled;
                opposite.erase(it);
            }
            else
            {
                resting->status = OrderStatus::PartiallyFilled;
            }
        }
    };

    if (incoming_side == Side::Buy)
        match_against(book_.sells());
    else
        match_against(book_.buys());

    if (incoming->remaining() == 0)
    {
        incoming->status = OrderStatus::Filled;
        return trades;
    }

    incoming->status = OrderStatus::New;
    book_.add(std::move(incoming));
    return trades;
}

template class MatchingEngine<double, int>;
template class MatchingEngine<double, std::int64_t>;
} // namespace hft::phase4

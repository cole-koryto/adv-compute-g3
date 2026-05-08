#pragma once

#include "Trade.hpp"

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

namespace hft::phase4
{
template <typename PriceType, typename OrderIdType>
class TradeLogger
{
public:
    using TradeT = Trade<PriceType, OrderIdType>;

    explicit TradeLogger(std::size_t reserve_trades = 1'000'000) { trades_.reserve(reserve_trades); }

    void log(TradeT trade) { trades_.push_back(std::move(trade)); }

    std::size_t size() const noexcept { return trades_.size(); }

    void flush_csv(const std::string &path);

private:
    std::vector<TradeT> trades_;
};
} // namespace hft::phase4


#include "../include/TradeLogger.hpp"

#include <iomanip>
#include <ostream>

namespace hft::phase4
{
template <typename PriceType, typename OrderIdType>
void TradeLogger<PriceType, OrderIdType>::flush_csv(const std::string &path)
{
    std::ofstream out(path);
    out << "symbol,buy_order_id,sell_order_id,price,quantity\n";
    out << std::setprecision(12);
    for (const auto &t : trades_)
        out << t.symbol << ',' << t.buy_order_id << ',' << t.sell_order_id << ',' << t.price << ',' << t.quantity << '\n';
}

template class TradeLogger<double, int>;
template class TradeLogger<double, std::int64_t>;
} // namespace hft::phase4


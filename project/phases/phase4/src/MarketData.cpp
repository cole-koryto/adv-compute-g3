#include "../include/MarketData.hpp"

namespace hft::phase4
{
MarketDataFeed::MarketDataFeed(std::uint64_t seed)
    : symbols_({"AAPL", "MSFT", "NVDA", "AMZN", "GOOG"}),
      rng_(seed),
      symbol_dist_(0, symbols_.size() - 1),
      price_delta_(0.0, 0.05)
{
}

MarketData MarketDataFeed::next_tick()
{
    mid_ += price_delta_(rng_);
    if (mid_ < 1.0)
        mid_ = 1.0;

    MarketData tick;
    tick.symbol = symbols_[symbol_dist_(rng_)];
    tick.bid_price = mid_ - (spread_ * 0.5);
    tick.ask_price = mid_ + (spread_ * 0.5);
    tick.timestamp = std::chrono::high_resolution_clock::now();
    return tick;
}
} // namespace hft::phase4


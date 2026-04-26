#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace hft::phase4
{
struct alignas(64) MarketData
{
    std::string symbol;
    double bid_price = 0.0;
    double ask_price = 0.0;
    std::chrono::high_resolution_clock::time_point timestamp{};
};

class MarketDataFeed
{
public:
    explicit MarketDataFeed(std::uint64_t seed = 1);

    MarketData next_tick();

private:
    std::vector<std::string> symbols_;
    std::mt19937_64 rng_;
    std::uniform_int_distribution<std::size_t> symbol_dist_;
    std::normal_distribution<double> price_delta_;
    double mid_ = 100.0;
    double spread_ = 0.02;
};
} // namespace hft::phase4


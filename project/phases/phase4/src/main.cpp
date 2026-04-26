#include "../include/MarketData.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/Timer.hpp"
#include "../include/TradeLogger.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory_resource>
#include <numeric>
#include <random>
#include <vector>

namespace
{
void analyze_latencies(std::vector<long long> &latencies)
{
    if (latencies.empty())
        return;

    std::sort(latencies.begin(), latencies.end());

    const auto min_v = latencies.front();
    const auto max_v = latencies.back();
    const double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / static_cast<double>(latencies.size());

    double variance = 0.0;
    for (const auto l : latencies)
        variance += (static_cast<double>(l) - mean) * (static_cast<double>(l) - mean);
    const double stddev = std::sqrt(variance / static_cast<double>(latencies.size()));

    const std::size_t p99_idx = static_cast<std::size_t>(static_cast<double>(latencies.size() - 1) * 0.99);
    const auto p99 = latencies[p99_idx];

    std::cout << "Tick-to-Trade Latency (nanoseconds)\n";
    std::cout << "Count: " << latencies.size() << "\n";
    std::cout << "Min:   " << min_v << "\n";
    std::cout << "Max:   " << max_v << "\n";
    std::cout << "Mean:  " << mean << "\n";
    std::cout << "StdDev:" << stddev << "\n";
    std::cout << "P99:   " << p99 << "\n";
}

std::uint64_t parse_u64(const char *s, std::uint64_t fallback)
{
    if (!s)
        return fallback;
    char *end = nullptr;
    const auto v = std::strtoull(s, &end, 10);
    return (end && *end == '\0') ? static_cast<std::uint64_t>(v) : fallback;
}
} // namespace

int main(int argc, char **argv)
{
    std::uint64_t ticks = 100'000;
    std::uint64_t seed = 1;

    for (int i = 1; i + 1 < argc; i += 2)
    {
        const std::string_view key(argv[i]);
        if (key == "--ticks")
            ticks = parse_u64(argv[i + 1], ticks);
        else if (key == "--seed")
            seed = parse_u64(argv[i + 1], seed);
    }

    // Memory pools for book nodes + order objects.
    std::vector<std::byte> buffer(8 * 1024 * 1024);
    std::pmr::monotonic_buffer_resource mono(buffer.data(), buffer.size());
    std::pmr::unsynchronized_pool_resource pool(&mono);

    using PriceT = double;
    using OrderIdT = std::int64_t;

    hft::phase4::MarketDataFeed feed(seed);
    hft::phase4::OrderBook<PriceT, OrderIdT> book(&pool);
    hft::phase4::OrderManager<OrderIdT> oms;
    hft::phase4::MatchingEngine<PriceT, OrderIdT> engine(book, oms);
    hft::phase4::TradeLogger<PriceT, OrderIdT> logger;

    std::vector<long long> latencies;
    latencies.reserve(static_cast<std::size_t>(ticks));

    for (std::uint64_t i = 0; i < ticks; ++i)
    {
        hft::phase4::Timer timer;
        timer.start();

        const auto tick = feed.next_tick();
        const auto side = (i % 2 == 0) ? hft::phase4::Side::Buy : hft::phase4::Side::Sell;
        const PriceT px = (side == hft::phase4::Side::Buy) ? tick.ask_price : tick.bid_price;

        auto order = book.make_order(static_cast<OrderIdT>(i + 1), tick.symbol, side, px, 100);
        oms.on_new(order->id, order->quantity);

        auto trades = engine.submit(std::move(order), tick.timestamp);
        for (auto &t : trades)
            logger.log(std::move(t));

        latencies.push_back(timer.stop_ns());
    }

    analyze_latencies(latencies);
    std::cout << "Trades logged: " << logger.size() << "\n";
    return 0;
}

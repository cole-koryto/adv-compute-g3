#include "../include/MarketData.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/Timer.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory_resource>
#include <string_view>
#include <vector>

namespace
{
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
    std::uint64_t ticks = 10'000;
    std::uint64_t seed = 1;

    for (int i = 1; i + 1 < argc; i += 2)
    {
        const std::string_view key(argv[i]);
        if (key == "--ticks")
            ticks = parse_u64(argv[i + 1], ticks);
        else if (key == "--seed")
            seed = parse_u64(argv[i + 1], seed);
    }

    std::vector<std::byte> buffer(2 * 1024 * 1024);
    std::pmr::monotonic_buffer_resource mono(buffer.data(), buffer.size());
    std::pmr::unsynchronized_pool_resource pool(&mono);

    using PriceT = double;
    using OrderIdT = std::int64_t;

    hft::phase4::MarketDataFeed feed(seed);
    hft::phase4::OrderBook<PriceT, OrderIdT> book(&pool);
    hft::phase4::OrderManager<OrderIdT> oms;
    hft::phase4::MatchingEngine<PriceT, OrderIdT> engine(book, oms);

    std::vector<long long> latencies;
    latencies.reserve(static_cast<std::size_t>(ticks));

    for (std::uint64_t i = 0; i < ticks; ++i)
    {
        hft::phase4::Timer timer;
        timer.start();

        const auto tick = feed.next_tick();
        const auto side = (i % 2 == 0) ? hft::phase4::Side::Buy : hft::phase4::Side::Sell;
        const PriceT px = (side == hft::phase4::Side::Buy) ? tick.ask_price : tick.bid_price;

        auto order = book.make_order(static_cast<OrderIdT>(i + 1), tick.symbol, side, px, 10);
        oms.on_new(order->id, order->quantity);

        (void)engine.submit(std::move(order), tick.timestamp);
        latencies.push_back(timer.stop_ns());
    }

    long long sum = 0;
    for (const auto l : latencies)
        sum += l;
    const auto mean = static_cast<double>(sum) / static_cast<double>(latencies.size());
    std::cout << "Mean tick-to-trade latency (ns): " << mean << "\n";
    std::cout << "Active OMS states: " << oms.active_count() << "\n";
    return 0;
}

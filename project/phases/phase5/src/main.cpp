#include "../include/order_book.hpp"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
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

std::string parse_string(const char *s, const std::string &fallback)
{
    if (!s)
        return fallback;
    return std::string(s);
}

struct BenchResult
{
    std::size_t n = 0;
    double add_s = 0.0;
    double modify_s = 0.0;
    double delete_s = 0.0;
};

BenchResult bench_once(std::size_t n, std::uint64_t seed)
{
    using Clock = std::chrono::high_resolution_clock;
    using Sec = std::chrono::duration<double>;

    hft::phase5::OrderBook book;
    book.reserve(n);

    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> price_dist(50.0, 100.0);
    std::uniform_int_distribution<int> qty_dist(1, 500);

    std::vector<std::string> ids;
    ids.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
        ids.push_back("ORD" + std::to_string(i));

    // Add orders
    const auto add_start = Clock::now();
    for (std::size_t i = 0; i < n; ++i)
    {
        const double px = price_dist(rng);
        const int qty = qty_dist(rng);
        book.add_order(ids[i], px, qty, true);
    }
    const auto add_end = Clock::now();

    // Modify half of them
    const auto mod_start = Clock::now();
    for (std::size_t i = 0; i < n; i += 2)
    {
        const double new_px = price_dist(rng);
        const int new_qty = qty_dist(rng);
        (void)book.modify_order(ids[i], new_px, new_qty);
    }
    const auto mod_end = Clock::now();

    // Delete all
    const auto del_start = Clock::now();
    for (std::size_t i = 0; i < n; ++i)
        (void)book.delete_order(ids[i]);
    const auto del_end = Clock::now();

    BenchResult r;
    r.n = n;
    r.add_s = std::chrono::duration_cast<Sec>(add_end - add_start).count();
    r.modify_s = std::chrono::duration_cast<Sec>(mod_end - mod_start).count();
    r.delete_s = std::chrono::duration_cast<Sec>(del_end - del_start).count();
    return r;
}
} // namespace

int main(int argc, char **argv)
{
    std::uint64_t seed = 1;
    std::string out_path = "phase5_bench.csv";

    for (int i = 1; i + 1 < argc; i += 2)
    {
        const std::string_view key(argv[i]);
        if (key == "--seed")
            seed = parse_u64(argv[i + 1], seed);
        else if (key == "--out")
            out_path = parse_string(argv[i + 1], out_path);
    }

    const std::vector<std::size_t> sizes = {1000, 5000, 10000, 50000, 100000};

    std::vector<BenchResult> results;
    results.reserve(sizes.size());

    for (std::size_t i = 0; i < sizes.size(); ++i)
        results.push_back(bench_once(sizes[i], seed + i));

    std::ofstream out(out_path);
    out << "orders,add_s,modify_s,delete_s,total_s\n";
    for (const auto &r : results)
    {
        const double total = r.add_s + r.modify_s + r.delete_s;
        out << r.n << ',' << r.add_s << ',' << r.modify_s << ',' << r.delete_s << ',' << total << '\n';
    }

    std::cout << "Wrote: " << out_path << "\n";
    for (const auto &r : results)
    {
        const double total = r.add_s + r.modify_s + r.delete_s;
        std::cout << "N=" << r.n << " total=" << total << "s"
                  << " (add " << r.add_s << "s"
                  << ", modify " << r.modify_s << "s"
                  << ", delete " << r.delete_s << "s)\n";
    }
    return 0;
}

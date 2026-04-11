#include <vector>
#include <unordered_map>
#include <array>
#include <iostream>
#include <numeric>

class TradingEngine {
public:
    // I added signal_counts to help us keep track of how many signals fired in our member initialization array
    TradingEngine(const std::vector<MarketData>& feed)
        : market_data(feed), signal_counts{0,0,0,0}
    {
        // vectors can reallocate when they grow. If we reserve memory for performance, we can avoid this issue
        orders.reserve(feed.size());
        latencies.reserve(feed.size());
    }

    void process() {
        for (const auto& tick : market_data) {
            updateHistory(tick);

            bool buy = false;
            bool sell = false;
            // I changed the assignment logic from buy = true into storing the signal as an int. 
            // We can count how often each signal fires and store the initial signal that was associated with the order as well
            int first_signal = 0;

            if (signal1(tick)) {
                buy = true;
                if (first_signal == 0) first_signal = 1;
                signal_counts[1]++;
            }

            if (signal2(tick)) {
                double avg = getAvg(tick.instrument_id);
                if (tick.price < avg) buy = true;
                else sell = true;

                if (first_signal == 0) first_signal = 2;
                signal_counts[2]++;
            }

            if (signal3(tick)) {
                buy = true;
                if (first_signal == 0) first_signal = 3;
                signal_counts[3]++;
            }

            if (buy || sell) {
                auto now = Clock::now();

                Order o;
                o.instrument_id = tick.instrument_id;
                o.price = tick.price + (buy ? 0.01 : -0.01);
                o.is_buy = buy;
                o.timestamp = now;
                // I also added triggering_signal to first signal for later.
                o.triggering_signal = first_signal;

                orders.push_back(o);

                auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    now - tick.timestamp).count();

                latencies.push_back(latency);
            }
        }
    }

    void reportStats() const {
        long long sum = 0, max_latency = 0;

        for (auto l : latencies) {
            sum += l;
            if (l > max_latency) max_latency = l;
        }

        std::cout << "\n--- Performance Report ---\n";
        std::cout << "Total Market Ticks Processed: " << market_data.size() << "\n";
        std::cout << "Total Orders Placed: " << orders.size() << "\n";
        std::cout << "Average Tick-to-Trade Latency (ns): "
                  << (latencies.empty() ? 0 : sum / latencies.size()) << "\n";
        std::cout << "Maximum Tick-to-Trade Latency (ns): " << max_latency << "\n";

        std::cout << "\n--- Signal Contributions ---\n";
        std::cout << "Signal 1: " << signal_counts[1] << "\n";
        std::cout << "Signal 2: " << signal_counts[2] << "\n";
        std::cout << "Signal 3: " << signal_counts[3] << "\n";
    }

private:
    const std::vector<MarketData>& market_data;
    std::vector<Order> orders;
    std::vector<long long> latencies;
    std::unordered_map<int, std::vector<double>> price_history;
    std::array<int, 4> signal_counts;

    void updateHistory(const MarketData& tick) {
        auto& hist = price_history[tick.instrument_id];
        hist.push_back(tick.price);
        if (hist.size() > 10) hist.erase(hist.begin());
    }

    double getAvg(int id) const {
        // I saw that the current initial implementation directl indexed the map everywhere.
        // finding it initially was cleaner to me and avoided accidental insertions that could occur in large scale/during runs
        auto it = price_history.find(id);
        if (it == price_history.end() || it->second.empty()) return 0;

        const auto& hist = it->second;
        double sum = std::accumulate(hist.begin(), hist.end(), 0.0);
        return sum / hist.size();
    }

    bool signal1(const MarketData& tick) const {
        return tick.price < 105.0 || tick.price > 195.0;
    }

    bool signal2(const MarketData& tick) const {
        auto it = price_history.find(tick.instrument_id);
        if (it == price_history.end() || it->second.size() < 5) return false;

        double avg = getAvg(tick.instrument_id);
        return tick.price < avg * 0.98 || tick.price > avg * 1.02;
    }

    bool signal3(const MarketData& tick) const {
        auto it = price_history.find(tick.instrument_id);
        if (it == price_history.end() || it->second.size() < 3) return false;

        const auto& hist = it->second;
        double d1 = hist[hist.size()-2] - hist[hist.size()-3];
        double d2 = hist[hist.size()-1] - hist[hist.size()-2];

        return d1 > 0 && d2 > 0;
    }
};
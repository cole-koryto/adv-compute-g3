#include <vector>
#include <random>

class MarketDataFeed {
public:
    MarketDataFeed(std::vector<MarketData>& ref) : data(ref) {}

    void generateData(int num_ticks) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> price_dist(100.0, 200.0);

        data.reserve(num_ticks);

        for (int i = 0; i < num_ticks; ++i) {
            MarketData md;
            md.instrument_id = i % 10;
            md.price = price_dist(gen);
            md.timestamp = Clock::now();
            data.push_back(md);
        }
    }

private:
    std::vector<MarketData>& data;
};
#include <iostream>
#include <vector>
#include "MarketData.cpp"
#include "MarketDataFeed.cpp"
#include "TradingEngine.cpp"

int main() {

    std::vector<MarketData> feed;
    MarketDataFeed generator(feed);

    auto start = Clock::now();

    generator.generateData(100000);

    TradingEngine engine(feed);
    engine.process();

    auto end = Clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    engine.reportStats();
    std::cout << "Total Runtime (ms): " << runtime << "\n";

    return 0;
}
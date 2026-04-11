#include <chrono>

// use a high resolution clock and have a shared data sturcture
using Clock = std::chrono::high_resolution_clock;

struct alignas(64) MarketData {
    int instrument_id;
    double price;
    Clock::time_point timestamp;
};

struct alignas(64) Order {
    int instrument_id;
    double price;
    bool is_buy;
    Clock::time_point timestamp;
    // I decided to add this to see which signal was responsible for an order. 
    // It would be helpful, because in the writeup we do have to answer that question
    int triggering_signal; 
};
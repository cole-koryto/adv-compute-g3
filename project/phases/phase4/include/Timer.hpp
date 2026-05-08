#pragma once

#include <chrono>

namespace hft::phase4
{
class Timer
{
public:
    using Clock = std::chrono::high_resolution_clock;

    void start() { start_ = Clock::now(); }

    long long stop_ns() const
    {
        const auto end = Clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_).count();
    }

private:
    Clock::time_point start_{};
};
} // namespace hft::phase4


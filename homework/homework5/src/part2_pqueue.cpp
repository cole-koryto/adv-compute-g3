#include "binary_heap.hpp"
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>

using clock_type = std::chrono::high_resolution_clock;

template <typename Fn> double time_run(Fn &&fn) {
  auto t0 = clock_type::now();
  fn();
  auto t1 = clock_type::now();
  return std::chrono::duration<double>(t1 - t0).count();
}

struct Order {
  double price;
  int timestamp;
  bool operator<(const Order &other) const { return price < other.price; }
  Order(double p, int ts) : price(p), timestamp(ts) {}
};

int main() {
  constexpr std::size_t N = 2 << 20;

  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(0.0, 1000.0);

  std::vector<Order> orders;
  orders.reserve(N);
  for (std::size_t i = 0; i < N; ++i)
    orders.emplace_back(dist(rng), static_cast<int>(i));

  BinaryHeap<Order> bh{};
  bh.reserve(N);

  std::vector<Order> pq_buf;
  pq_buf.reserve(N);
  std::priority_queue<Order, std::vector<Order>> pq(std::less<Order>{},
                                                    std::move(pq_buf));

  volatile double sink = 0.0;

  const double t_bh_push = time_run([&] {
    for (const auto &o : orders)
      bh.push(o);
  });
  const double t_pq_push = time_run([&] {
    for (const auto &o : orders)
      pq.push(o);
  });
  const double t_bh_pop = time_run([&] {
    while (!bh.empty()) {
      sink += bh.top().price;
      bh.pop();
    }
  });
  const double t_pq_pop = time_run([&] {
    while (!pq.empty()) {
      sink += pq.top().price;
      pq.pop();
    }
  });

  std::cout << "N = " << N << "\n\n";

  std::cout << std::left << std::setw(22) << "variant" << std::right
            << std::setw(14) << "push (s)" << std::setw(14) << "pop (s)"
            << "\n";
  std::cout << std::string(50, '-') << "\n";

  auto row = [&](const std::string &name, double t_push, double t_pop) {
    std::cout << std::left << std::setw(22) << name << std::right << std::fixed
              << std::setprecision(4) << std::setw(14) << t_push
              << std::setw(14) << t_pop << "\n";
  };

  row("BinaryHeap", t_bh_push, t_bh_pop);
  row("std::priority_queue", t_pq_push, t_pq_pop);
}

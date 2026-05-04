#include "order_book.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int main() {
  constexpr int N_ORDERS = 1 << 19; // ~500k orders
  constexpr int N_BBO = 1 << 12;    // ~4k BBO queries

  std::mt19937_64 rng(42);
  std::uniform_real_distribution<double> price_dist(90.0, 110.0);
  std::uniform_int_distribution<int> qty_dist(1, 1000);

  // pre-generated workload: same orders fed to both books
  std::vector<Order> orders;
  orders.reserve(N_ORDERS);
  for (int i = 1; i <= N_ORDERS; ++i)
    orders.push_back({i, price_dist(rng), qty_dist(rng), (i & 1) ? 'B' : 'S'});

  OrderBook hybrid;
  OrderBookNaive naive;

  // --- insert ---
  auto t1 = std::chrono::high_resolution_clock::now();
  for (const auto &o : orders)
    hybrid.add_order(o.id, o.price, o.quantity, o.side);
  auto t2 = std::chrono::high_resolution_clock::now();
  for (const auto &o : orders)
    naive.add_order(o.id, o.price, o.quantity, o.side);
  auto t3 = std::chrono::high_resolution_clock::now();

  // --- best bid / best ask ---
  volatile double sink = 0.0;
  auto t4 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N_BBO; ++i) {
    auto bb = hybrid.best_bid();
    auto ba = hybrid.best_ask();
    if (bb)
      sink += *bb;
    if (ba)
      sink += *ba;
  }
  auto t5 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N_BBO; ++i) {
    auto bb = naive.best_bid();
    auto ba = naive.best_ask();
    if (bb)
      sink += *bb;
    if (ba)
      sink += *ba;
  }
  auto t6 = std::chrono::high_resolution_clock::now();

  using sec = std::chrono::duration<double>;
  std::cout << "insert " << N_ORDERS << " orders\n";
  std::cout << "  hybrid (map+hash): " << sec(t2 - t1).count() << " s\n";
  std::cout << "  naive  (hash only): " << sec(t3 - t2).count() << " s\n";
  std::cout << "BBO " << N_BBO << " queries\n";
  std::cout << "  hybrid (map+hash): " << sec(t5 - t4).count() << " s\n";
  std::cout << "  naive  (hash only): " << sec(t6 - t5).count() << " s\n";
}

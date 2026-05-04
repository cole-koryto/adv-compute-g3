#include "robin_hood_map.hpp"
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using clock_type = std::chrono::high_resolution_clock;

template <typename Fn> double time_run(Fn &&fn) {
  auto t0 = clock_type::now();
  fn();
  auto t1 = clock_type::now();
  return std::chrono::duration<double>(t1 - t0).count();
}

int main() {
  constexpr std::size_t N = 2 << 20;

  std::vector<std::string> keys;
  keys.reserve(N);
  for (std::size_t i = 0; i < N; ++i)
    keys.push_back(std::to_string(i));

  RobinHoodMap<int> rh(N << 1);
  std::unordered_map<std::string, int> um;
  um.reserve(N);

  volatile std::size_t sink = 0;

  const double t_rh_insert = time_run([&] {
    for (std::size_t i = 0; i < N; ++i)
      rh.insert(keys[i], static_cast<int>(i));
  });
  const double t_um_insert = time_run([&] {
    for (std::size_t i = 0; i < N; ++i)
      um[keys[i]] = static_cast<int>(i);
  });
  const double t_rh_lookup = time_run([&] {
    for (const auto &k : keys) {
      auto *v = rh.find(k);
      if (v)
        sink += *v;
    }
  });
  const double t_um_lookup = time_run([&] {
    for (const auto &k : keys) {
      auto it = um.find(k);
      if (it != um.end())
        sink += it->second;
    }
  });

  std::cout << "N = " << N << "\n\n";

  std::cout << std::left << std::setw(22) << "variant" << std::right
            << std::setw(14) << "insert (s)" << std::setw(14) << "lookup (s)"
            << "\n";
  std::cout << std::string(50, '-') << "\n";

  auto row = [&](const std::string &name, double t_ins, double t_look) {
    std::cout << std::left << std::setw(22) << name << std::right << std::fixed
              << std::setprecision(4) << std::setw(14) << t_ins << std::setw(14)
              << t_look << "\n";
  };

  row("RobinHoodMap", t_rh_insert, t_rh_lookup);
  row("std::unordered_map", t_um_insert, t_um_lookup);
}

#include "moving_average_simd.hpp"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
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

double max_diff(const std::vector<double> &a, const std::vector<double> &b) {
  double m = 0.0;
  for (size_t i = 0; i < a.size(); ++i) {
    double d = std::fabs(a[i] - b[i]);
    if (d > m)
      m = d;
  }
  return m;
}

int main() {
  constexpr std::size_t N = 1 << 24; // 16M doubles
  constexpr std::size_t W = 512;
  const std::size_t M = N - W + 1;

  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(0.0, 1000.0);

  std::vector<double> prices(N);
  for (auto &p : prices)
    p = dist(rng);

  std::vector<double> out_scalar(M);
  std::vector<double> out_simd1(M);
  std::vector<double> out_simd2(M);
  std::vector<double> out_simd4(M);

  const double t_scalar = time_run(
      [&] { moving_average_scalar(prices.data(), N, W, out_scalar.data()); });
  const double t_simd1 = time_run(
      [&] { moving_average_simd_1(prices.data(), N, W, out_simd1.data()); });
  const double t_simd2 = time_run(
      [&] { moving_average_simd_2(prices.data(), N, W, out_simd2.data()); });
  const double t_simd4 = time_run(
      [&] { moving_average_simd_4(prices.data(), N, W, out_simd4.data()); });

  // Volatile sink: keep all 4 result arrays live.
  volatile double sink = 0.0;
  for (std::size_t i = 0; i < M; ++i)
    sink += out_scalar[i] + out_simd1[i] + out_simd2[i] + out_simd4[i];

  const double d1 = max_diff(out_scalar, out_simd1);
  const double d2 = max_diff(out_scalar, out_simd2);
  const double d4 = max_diff(out_scalar, out_simd4);

  std::cout << "N = " << N << ", W = " << W << ", M = " << M << "\n\n";

  std::cout << std::left << std::setw(18) << "variant" << std::right
            << std::setw(12) << "time (s)" << std::setw(12) << "speedup"
            << std::setw(16) << "max |scalar-x|" << "\n";
  std::cout << std::string(58, '-') << "\n";

  auto row = [&](const std::string &name, double t, double diff) {
    std::cout << std::left << std::setw(18) << name << std::right << std::fixed
              << std::setprecision(4) << std::setw(12) << t << std::setw(12)
              << (t_scalar / t) << std::setw(16) << std::scientific
              << std::setprecision(2) << diff << std::defaultfloat << "\n";
  };

  std::cout << std::left << std::setw(18) << "scalar" << std::right
            << std::fixed << std::setprecision(4) << std::setw(12) << t_scalar
            << std::setw(12) << 1.0 << std::setw(16) << "-" << "\n";
  row("simd_1 (1 acc)", t_simd1, d1);
  row("simd_2 (2 acc)", t_simd2, d2);
  row("simd_4 (4 acc)", t_simd4, d4);
}

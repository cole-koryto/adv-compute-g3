#ifndef MOVING_AVERAGE_SIMD_HPP
#define MOVING_AVERAGE_SIMD_HPP

#define SIMDE_ENABLE_NATIVE_ALIASES
#include <cstddef>
#include <simde/x86/avx2.h>

inline void moving_average_scalar(const double *prices, size_t n, size_t w,
                                  double *out) {
  double wd = static_cast<double>(w);
  for (size_t i = 0; i + w <= n; ++i) {
    double sum = 0.0;
    for (size_t j = 0; j < w; ++j)
      sum += prices[i + j];
    out[i] = sum / wd;
  }
}

// Scalar tail shared by all SIMD variants — handles the (M % 4) outputs the
// outer loop's parallel-windows vectorization doesn't cover.
inline void moving_average_simd_tail(const double *prices, size_t n, size_t w,
                                     double *out, size_t i_start) {
  double wd_inv = 1.0 / w;
  for (size_t i = i_start; i + w <= n; ++i) {
    double sum = 0.0;
    for (size_t j = 0; j < w; ++j)
      sum += prices[i + j];
    out[i] = sum * wd_inv;
  }
}

// SIMD with 1 accumulator
inline void moving_average_simd_1(const double *prices, size_t n, size_t w,
                                  double *out) {
  const size_t M = n - w + 1;
  __m256d factor = _mm256_set1_pd(1.0 / w);

  for (size_t i = 0; i + 4 <= M; i += 4) {
    __m256d s = _mm256_setzero_pd();
    for (size_t j = 0; j < w; ++j)
      s = _mm256_add_pd(s, _mm256_loadu_pd(&prices[i + j]));
    _mm256_storeu_pd(&out[i], _mm256_mul_pd(s, factor));
  }

  moving_average_simd_tail(prices, n, w, out, (M / 4) * 4);
}

// SIMD with 2 accumulators.
inline void moving_average_simd_2(const double *prices, size_t n, size_t w,
                                  double *out) {
  const size_t M = n - w + 1;
  __m256d factor = _mm256_set1_pd(1.0 / w);

  for (size_t i = 0; i + 4 <= M; i += 4) {
    __m256d s0 = _mm256_setzero_pd();
    __m256d s1 = _mm256_setzero_pd();

    size_t j = 0;
    for (; j + 2 <= w; j += 2) {
      s0 = _mm256_add_pd(s0, _mm256_loadu_pd(&prices[i + j]));
      s1 = _mm256_add_pd(s1, _mm256_loadu_pd(&prices[i + j + 1]));
    }
    for (; j < w; ++j)
      s0 = _mm256_add_pd(s0, _mm256_loadu_pd(&prices[i + j]));

    __m256d s = _mm256_add_pd(s0, s1);
    _mm256_storeu_pd(&out[i], _mm256_mul_pd(s, factor));
  }

  moving_average_simd_tail(prices, n, w, out, (M / 4) * 4);
}

// SIMD with 4 accumulators
inline void moving_average_simd_4(const double *prices, size_t n, size_t w,
                                  double *out) {
  const size_t M = n - w + 1;
  __m256d factor = _mm256_set1_pd(1.0 / w);

  for (size_t i = 0; i + 4 <= M; i += 4) {
    __m256d s0 = _mm256_setzero_pd();
    __m256d s1 = _mm256_setzero_pd();
    __m256d s2 = _mm256_setzero_pd();
    __m256d s3 = _mm256_setzero_pd();

    size_t j = 0;
    for (; j + 4 <= w; j += 4) {
      s0 = _mm256_add_pd(s0, _mm256_loadu_pd(&prices[i + j]));
      s1 = _mm256_add_pd(s1, _mm256_loadu_pd(&prices[i + j + 1]));
      s2 = _mm256_add_pd(s2, _mm256_loadu_pd(&prices[i + j + 2]));
      s3 = _mm256_add_pd(s3, _mm256_loadu_pd(&prices[i + j + 3]));
    }
    for (; j < w; ++j)
      s0 = _mm256_add_pd(s0, _mm256_loadu_pd(&prices[i + j]));

    __m256d s = _mm256_add_pd(_mm256_add_pd(s0, s1), _mm256_add_pd(s2, s3));
    _mm256_storeu_pd(&out[i], _mm256_mul_pd(s, factor));
  }

  moving_average_simd_tail(prices, n, w, out, (M / 4) * 4);
}

#endif // MOVING_AVERAGE_SIMD_HPP

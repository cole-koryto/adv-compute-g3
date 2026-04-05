#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <random>
#include "phase1.hpp"

bool check(const double *result, const double *expected, int size, const char *name)
{
    for (int i = 0; i < size; i++)
    {
        if (std::abs(result[i] - expected[i]) > 1e-9)
        {
            std::cout << "[FAIL] " << name << " — index " << i
                      << ": got " << result[i] << ", expected " << expected[i] << std::endl;
            return false;
        }
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}

int main(int argc, char *argv[])
{
    std::string mode = "test";
    if (argc > 1)
        mode = argv[1];

    if (mode == "bench")
    {
        const int RUNS = 20;
        const int SCALE = 10;
        int sizes[] = {64, 128, 256, 512, 1024};

        std::cout << "size,function,avg_ms,stddev_ms" << std::endl;

        for (int N : sizes)
        {
            // allocate matrices and vectors
            double *A_row, *A_col, *B, *B_T, *v, *mv_result, *mm_result;
            generate_random_matrix(N, N, A_row, SCALE);
            generate_random_matrix(N, N, A_col, SCALE);  // same random data is fine for col-major
            generate_random_matrix(N, N, B, SCALE);
            generate_random_matrix(N, N, B_T, SCALE);    // treat as pre-transposed B
            generate_random_vector(N, v, SCALE);
            mv_result = new double[N];
            mm_result = new double[N * N];

            // benchmark each function
            struct Bench
            {
                const char *name;
                std::vector<double> times;
            };
            Bench benches[] = {
                {"mv_row_major", {}},
                {"mv_col_major", {}},
                {"mm_naive", {}},
                {"mm_transposed_b", {}},
            };

            for (int r = 0; r < RUNS; r++)
            {
                auto t0 = std::chrono::high_resolution_clock::now();
                multiply_mv_row_major(A_row, N, N, v, mv_result);
                auto t1 = std::chrono::high_resolution_clock::now();
                multiply_mv_col_major(A_col, N, N, v, mv_result);
                auto t2 = std::chrono::high_resolution_clock::now();
                multiply_mm_naive(A_row, N, N, B, N, N, mm_result);
                auto t3 = std::chrono::high_resolution_clock::now();
                multiply_mm_transposed_b(A_row, N, N, B_T, N, N, mm_result);
                auto t4 = std::chrono::high_resolution_clock::now();

                benches[0].times.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
                benches[1].times.push_back(std::chrono::duration<double, std::milli>(t2 - t1).count());
                benches[2].times.push_back(std::chrono::duration<double, std::milli>(t3 - t2).count());
                benches[3].times.push_back(std::chrono::duration<double, std::milli>(t4 - t3).count());
            }

            for (auto &b : benches)
            {
                double sum = 0;
                for (double t : b.times)
                    sum += t;
                double mean = sum / b.times.size();

                double sq_sum = 0;
                for (double t : b.times)
                    sq_sum += (t - mean) * (t - mean);
                double stddev = std::sqrt(sq_sum / b.times.size());

                std::cout << N << "," << b.name << "," << mean << "," << stddev << std::endl;
            }

            delete[] A_row;
            delete[] A_col;
            delete[] B;
            delete[] B_T;
            delete[] v;
            delete[] mv_result;
            delete[] mm_result;
        }
        return 0;
    }

    if (mode == "bench-align")
    {
        const int RUNS = 20;
        const int SCALE = 10;
        int sizes[] = {64, 128, 256, 512, 1024};

        std::cout << "size,function,alloc,avg_ms,stddev_ms" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int N : sizes)
        {
            size_t mat_bytes = sizeof(double) * N * N;
            size_t vec_bytes = sizeof(double) * N;

            // unaligned allocation (new + offset by 1 byte via char* trick to force misalignment)
            double *uA = new double[N * N];
            double *uB = new double[N * N];
            double *uB_T = new double[N * N];
            double *uv = new double[N];
            double *umv_res = new double[N];
            double *umm_res = new double[N * N];

            // aligned allocation (64-byte boundary)
            double *aA = (double *)aligned_alloc(64, mat_bytes);
            double *aB = (double *)aligned_alloc(64, mat_bytes);
            double *aB_T = (double *)aligned_alloc(64, mat_bytes);
            double *av = (double *)aligned_alloc(64, vec_bytes);
            double *amv_res = (double *)aligned_alloc(64, vec_bytes);
            double *amm_res = (double *)aligned_alloc(64, mat_bytes);

            // fill with same random data
            for (int i = 0; i < N * N; i++)
            {
                double val = dist(gen) * SCALE;
                uA[i] = val; aA[i] = val;
                val = dist(gen) * SCALE;
                uB[i] = val; aB[i] = val;
                val = dist(gen) * SCALE;
                uB_T[i] = val; aB_T[i] = val;
            }
            for (int i = 0; i < N; i++)
            {
                double val = dist(gen) * SCALE;
                uv[i] = val; av[i] = val;
            }

            struct Bench
            {
                const char *name;
                const char *alloc;
                std::vector<double> times;
            };
            Bench benches[] = {
                {"mm_naive", "unaligned", {}},
                {"mm_naive", "aligned", {}},
                {"mm_transposed_b", "unaligned", {}},
                {"mm_transposed_b", "aligned", {}},
            };

            for (int r = 0; r < RUNS; r++)
            {
                auto t0 = std::chrono::high_resolution_clock::now();
                multiply_mm_naive(uA, N, N, uB, N, N, umm_res);
                auto t1 = std::chrono::high_resolution_clock::now();
                multiply_mm_naive(aA, N, N, aB, N, N, amm_res);
                auto t2 = std::chrono::high_resolution_clock::now();
                multiply_mm_transposed_b(uA, N, N, uB_T, N, N, umm_res);
                auto t3 = std::chrono::high_resolution_clock::now();
                multiply_mm_transposed_b(aA, N, N, aB_T, N, N, amm_res);
                auto t4 = std::chrono::high_resolution_clock::now();

                benches[0].times.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
                benches[1].times.push_back(std::chrono::duration<double, std::milli>(t2 - t1).count());
                benches[2].times.push_back(std::chrono::duration<double, std::milli>(t3 - t2).count());
                benches[3].times.push_back(std::chrono::duration<double, std::milli>(t4 - t3).count());
            }

            for (auto &b : benches)
            {
                double sum = 0;
                for (double t : b.times) sum += t;
                double mean = sum / b.times.size();
                double sq_sum = 0;
                for (double t : b.times) sq_sum += (t - mean) * (t - mean);
                double stddev = std::sqrt(sq_sum / b.times.size());
                std::cout << N << "," << b.name << "," << b.alloc << "," << mean << "," << stddev << std::endl;
            }

            delete[] uA; delete[] uB; delete[] uB_T;
            delete[] uv; delete[] umv_res; delete[] umm_res;
            std::free(aA); std::free(aB); std::free(aB_T);
            std::free(av); std::free(amv_res); std::free(amm_res);
        }
        return 0;
    }

    if (mode == "profile")
    {
        std::cout << "TODO: profiling" << std::endl;
        return 0;
    }

    // default: test mode
    int passed = 0;
    int total = 8;

    //  Matrix-Vector Tests

    // Test 1: MV Row-Major 2x2
    // A = [[1, 2], [3, 4]]  v = [5, 6]  =>  [17, 39]
    {
        double A[] = {1, 2, 3, 4};
        double v[] = {5, 6};
        double expected[] = {17, 39};
        double result[2];
        multiply_mv_row_major(A, 2, 2, v, result);
        if (check(result, expected, 2, "mv_row_major 2x2"))
            passed++;
    }

    // Test 2: MV Row-Major 2x3 (non-square)
    // A = [[1, 2, 3], [4, 5, 6]]  v = [1, 2, 3]  =>  [14, 32]
    {
        double A[] = {1, 2, 3, 4, 5, 6};
        double v[] = {1, 2, 3};
        double expected[] = {14, 32};
        double result[2];
        multiply_mv_row_major(A, 2, 3, v, result);
        if (check(result, expected, 2, "mv_row_major 2x3"))
            passed++;
    }

    // Test 3: MV Col-Major 2x2
    // A = [[1, 2], [3, 4]] col-major: {1, 3, 2, 4}  v = [5, 6]  =>  [17, 39]
    {
        double A_col[] = {1, 3, 2, 4};
        double v[] = {5, 6};
        double expected[] = {17, 39};
        double result[2];
        multiply_mv_col_major(A_col, 2, 2, v, result);
        if (check(result, expected, 2, "mv_col_major 2x2"))
            passed++;
    }

    // Test 4: MV Col-Major 2x3 (non-square)
    // A = [[1, 2, 3], [4, 5, 6]] col-major: {1, 4, 2, 5, 3, 6}  v = [1, 2, 3]  =>  [14, 32]
    {
        double A_col[] = {1, 4, 2, 5, 3, 6};
        double v[] = {1, 2, 3};
        double expected[] = {14, 32};
        double result[2];
        multiply_mv_col_major(A_col, 2, 3, v, result);
        if (check(result, expected, 2, "mv_col_major 2x3"))
            passed++;
    }

    //  Matrix-Matrix Tests

    // Test 5: MM Naive 2x2
    // A = [[1, 2], [3, 4]]  B = [[5, 6], [7, 8]]  =>  [[19, 22], [43, 50]]
    {
        double A[] = {1, 2, 3, 4};
        double B[] = {5, 6, 7, 8};
        double expected[] = {19, 22, 43, 50};
        double result[4];
        multiply_mm_naive(A, 2, 2, B, 2, 2, result);
        if (check(result, expected, 4, "mm_naive 2x2"))
            passed++;
    }

    // Test 6: MM Naive non-square (2x3) x (3x2)
    // A = [[1, 2, 3], [4, 5, 6]]  B = [[7, 8], [9, 10], [11, 12]]
    // A*B = [[1*7+2*9+3*11, 1*8+2*10+3*12], [4*7+5*9+6*11, 4*8+5*10+6*12]]
    //     = [[58, 64], [139, 154]]
    {
        double A[] = {1, 2, 3, 4, 5, 6};
        double B[] = {7, 8, 9, 10, 11, 12};
        double expected[] = {58, 64, 139, 154};
        double result[4];
        multiply_mm_naive(A, 2, 3, B, 3, 2, result);
        if (check(result, expected, 4, "mm_naive (2x3)x(3x2)"))
            passed++;
    }

    // Test 7: MM Transposed B 2x2
    // A = [[1, 2], [3, 4]]  B = [[5, 6], [7, 8]]  B^T = [[5, 7], [6, 8]]
    // A*B = [[19, 22], [43, 50]]
    {
        double A[] = {1, 2, 3, 4};
        double B_T[] = {5, 7, 6, 8};
        double expected[] = {19, 22, 43, 50};
        double result[4];
        multiply_mm_transposed_b(A, 2, 2, B_T, 2, 2, result);
        if (check(result, expected, 4, "mm_transposed_b 2x2"))
            passed++;
    }

    // Test 8: MM Transposed B non-square (2x3) x (3x2)
    // A = [[1, 2, 3], [4, 5, 6]]  B = [[7, 8], [9, 10], [11, 12]]
    // B^T = [[7, 9, 11], [8, 10, 12]]  (2x3 row-major)
    // A*B = [[58, 64], [139, 154]]
    {
        double A[] = {1, 2, 3, 4, 5, 6};
        double B_T[] = {7, 9, 11, 8, 10, 12};
        double expected[] = {58, 64, 139, 154};
        double result[4];
        multiply_mm_transposed_b(A, 2, 3, B_T, 2, 3, result);
        if (check(result, expected, 4, "mm_transposed_b (2x3)x(3x2)"))
            passed++;
    }

    std::cout << "\n"
              << passed << "/" << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
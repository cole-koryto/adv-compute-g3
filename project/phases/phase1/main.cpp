#include <iostream>
#include <cmath>
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

int main()
{
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
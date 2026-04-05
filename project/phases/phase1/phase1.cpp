#include "phase1.hpp"
#include <iostream>
#include <random>

void print_matrix(const double *matrix, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_matrix_col_major(const double *matrix, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            std::cout << matrix[i + j * rows] << " ";
        }
        std::cout << std::endl;
    }
}

void generate_random_matrix(int rows, int cols, double *&matrix, int scale)
{
    // set up random environment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    // allocate data (contiguous block)
    matrix = new double[rows * cols];

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            matrix[i * cols + j] = dist(gen) * scale;
        }
    }
}

void generate_random_vector(int size, double *&vector, int scale)
{
    // set up random environment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    // allocate data (contiguous block)
    vector = new double[size];

    for (int i = 0; i < size; ++i)
    {
        vector[i] = dist(gen) * scale;
    }
}

// assumes row-major order
void multiply_mv_row_major(const double *matrix, int rows, int cols, const double *vector, double *result)
{
    if (!matrix || !vector || !result)
    {
        std::cerr << "Error: null pointer passed to multiply_mv_row_major" << std::endl;
        return;
    }
    if (rows <= 0 || cols <= 0)
    {
        std::cerr << "Error: invalid dimensions in multiply_mv_row_major" << std::endl;
        return;
    }
    for (int i = 0; i < rows; ++i)
    {
        result[i] = 0;
        for (int j = 0; j < cols; ++j)
        {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

// assumes matrix is stored in column-major order
void multiply_mv_col_major(const double *matrix, int rows, int cols, const double *vector, double *result)
{
    if (!matrix || !vector || !result)
    {
        std::cerr << "Error: null pointer passed to multiply_mv_col_major" << std::endl;
        return;
    }
    if (rows <= 0 || cols <= 0)
    {
        std::cerr << "Error: invalid dimensions in multiply_mv_col_major" << std::endl;
        return;
    }
    // stored [col1 ... coln]
    for (int i = 0; i < rows; i++)
    {
        result[i] = 0;
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i + j * rows] * vector[j];
        }
    }
}

void multiply_mm_naive(const double *matrixA, int rowsA, int colsA, const double *matrixB, int rowsB, int colsB, double *result)
{
    if (!matrixA || !matrixB || !result)
    {
        std::cerr << "Error: null pointer passed to multiply_mm_naive" << std::endl;
        return;
    }
    if (colsA != rowsB)
    {
        std::cerr << "Error: dimensions must be (m, n) x (n, k)" << std::endl;
        return;
    }

    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsB; j++)
        {
            result[i * colsB + j] = 0; // initialize result element
            // result will be rowsA x colsB in shape (length)
            // M[i][j] = result[i * colsB + j] = matrixA[i] * matrixB[j]
            for (int k = 0; k < colsA; k++)
            {
                result[i * colsB + j] += matrixA[i * colsA + k] * matrixB[j + k * colsB];
            }
        }
    }
}

void multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA, const double *matrixB_transposed, int rowsB, int colsB, double *result)
{
    if (!matrixA || !matrixB_transposed || !result)
    {
        std::cerr << "Error: null pointer passed to multiply_mm_transposed_b" << std::endl;
        return;
    }
    if (colsA != colsB)
    {
        std::cerr << "Error: dimensions must be (m, n) x (k, n) for transposed B" << std::endl;
        return;
    }

    // this time we got rowA x rowB instead of rowA x colB
    // result_ij = row_i of A dot row_j of B^T
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < rowsB; j++)
        {
            result[i * rowsB + j] = 0; // initialize result element
            for (int k = 0; k < colsA; k++)
            {
                result[i * rowsB + j] += matrixA[i * colsA + k] * matrixB_transposed[j * colsB + k];
            }
        }
    }
}
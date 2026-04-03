#include <iostream>
#include <random>

void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result)
{

}


void generate_random_matrix(int rows, int cols, double** matrix)
{
    // set up random environment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    // allocate data (contiguous block)
    double* data = new double[rows * cols];

    // allocate row pointers
    matrix = new double*[rows];

    // map each row
    for (int i = 0; i < rows; i++) {
        matrix[i] = data + i * cols;
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            matrix[i][j] = dist(gen);
        }
    }
}

int main()
{
    // Generate a large random matrix



    // Evaluate performance (according to Benchmarking in assignment)
    // Run Matrix-Vector Multiplication (Row-Major)

    // Run Matrix-Vector Multiplication (Column-Major)

    // Run Matrix-Matrix Multiplication (Naive)

    // Run Matrix-Matrix Multiplication (Transposed B)

    // Test correctness
    // Run Matrix-Vector Multiplication (Row-Major)

    // Run Matrix-Vector Multiplication (Column-Major)

    // Run Matrix-Matrix Multiplication (Naive)

    // Run Matrix-Matrix Multiplication (Transposed B)
}
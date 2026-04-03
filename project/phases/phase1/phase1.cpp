#include <iostream>
#include <random>
constexpr int SCALE = 100;

void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result)
{
    for (int i = 0; i < rows; ++i)
    {
        result[i] = 0;
        for (int j = 0; j < cols; ++j)
        {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

void generate_random_matrix(int rows, int cols, double*& matrix)
{
    // set up random environment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    // allocate data (contiguous block)
    matrix = new double[rows * cols]; // todo free data still

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            matrix[i * cols + j] = dist(gen) * SCALE;
        }
    }
}

void generate_random_vector(int size, double*& vector)
{
    // set up random environment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    // allocate data (contiguous block)
    vector = new double[size];

    for (int i = 0; i < size; ++i)
    {
        vector[i] = dist(gen) * SCALE;
    }
}

int main()
{
    const int ROWS = 2;
    const int COLS = 2;

    // Generate a random matrix
    double* matrix;
    generate_random_matrix(ROWS, COLS, matrix);
    std::cout<< "Matrix" << std::endl;
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            std::cout<< matrix[i * COLS + j] << std::endl;
        }
    }

    // Generate a random vector
    double* vector;
    generate_random_vector(COLS, vector);
    std::cout<< "Vector" << std::endl;
    for (int i = 0; i < ROWS; ++i)
    {
        std::cout<< vector[i] << std::endl;
    }

    // Evaluate performance (according to Benchmarking in assignment)
    // Run Matrix-Vector Multiplication (Row-Major)
    double* result = new double[ROWS];
    multiply_mv_row_major(matrix, ROWS, COLS, vector, result);
    std::cout<< "Matrix-Vector Multiplication Results: " << std::endl;
    for (int i = 0; i < ROWS; ++i)
    {
        std::cout<< result[i] << std::endl;
    }

    // Run Matrix-Vector Multiplication (Column-Major)

    // Run Matrix-Matrix Multiplication (Naive)

    // Run Matrix-Matrix Multiplication (Transposed B)

    // Test correctness
    // Run Matrix-Vector Multiplication (Row-Major)

    // Run Matrix-Vector Multiplication (Column-Major)

    // Run Matrix-Matrix Multiplication (Naive)

    // Run Matrix-Matrix Multiplication (Transposed B)

    delete[] matrix;
    delete[] vector;
    delete[] result;
}
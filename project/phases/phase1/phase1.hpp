#ifndef PHASE1_HPP
#define PHASE1_HPP

// helpers
void print_matrix(const double *matrix, int rows, int cols);

// prints a matrix in column-major order (for debugging)
void print_matrix_col_major(const double *matrix, int rows, int cols);

void generate_random_matrix(int rows, int cols, double *&matrix, int scale);

void generate_random_vector(int size, double *&vector, int scale);

// multiply matrix (row-major) with vector
void multiply_mv_row_major(const double *matrix, int rows, int cols, const double *vector, double *result);

// multiply matrix (column-major) with vector
void multiply_mv_col_major(const double *matrix, int rows, int cols, const double *vector, double *result);

// naive matrix-matrix multiplication (row-major)
void multiply_mm_naive(const double *matrixA, int rowsA, int colsA, const double *matrixB, int rowsB, int colsB, double *result);

// multiply matrix A with transposed matrix B (row-major)
void multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA, const double *matrixB_transposed, int rowsB, int colsB, double *result);

#endif // PHASE1_HPP
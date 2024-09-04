#pragma once

#include <vector>
#include <iostream>

class matrix {
    public:
        matrix(int rows, int cols);
        int get_rows() ;
        int get_cols();
        void add_row(std::vector<float> row);
        void add_col(std::vector<float> col);
        matrix* clone();
        std::vector<std::vector<float>> data;
};


matrix add_matrix(matrix a, matrix b);

matrix sub_matrix(matrix a, matrix b);

matrix mul_matrix(matrix a, matrix b);

matrix add_matrix_vector(matrix mat, matrix vect);

matrix mul_matrix_vector(matrix mat, matrix vect);

matrix scalar_multiply(matrix mat, float scalar);

matrix hadamard_product(matrix a, matrix b);

matrix transpose_matrix(matrix a);
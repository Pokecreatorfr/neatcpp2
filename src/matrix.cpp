#include <matrix.hpp>


matrix::matrix(int rows, int cols) {
    data.resize(rows);
    for (int i = 0; i < rows; i++) {
        data[i].resize(cols);
    }
}

int matrix::get_rows() {
    return data.size();
}

int matrix::get_cols() {
    return data[0].size();
}

void matrix::add_row(std::vector<float> row)
{
    if (row.size() != data[0].size()) {
        std::cerr << "Row size must match matrix column size" << std::endl;
    }
    data.push_back(row);
}

void matrix::add_col(std::vector<float> col)
{
    if (col.size() != data.size()) {
        std::cerr << "Column size must match matrix row size" << std::endl;
    }
    for (int i = 0; i < data.size(); i++) {
        data[i].push_back(col[i]);
    }
}

matrix *matrix::clone()
{
    matrix* result = new matrix(0,0);
    result->data = this->data;
    return result;
}

matrix add_matrix(matrix a, matrix b) {
    int arows = a.get_rows();
    int acols = a.get_cols();

    if (arows != b.get_rows() || acols != b.get_cols()) {
        std::cerr << "Matrix dimensions must match" << std::endl;
    }

    matrix result(arows, acols);

    for (int i = 0; i < arows; i++) {
        for (int j = 0; j < acols; j++) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }

    return result;
}

matrix sub_matrix(matrix a, matrix b) {
    int arows = a.get_rows();
    int acols = a.get_cols();

    if (arows != b.get_rows() || acols != b.get_cols()) {
        std::cerr << "Matrix dimensions must match" << std::endl;
    }

    matrix result(arows, acols);

    for (int i = 0; i < arows; i++) {
        for (int j = 0; j < acols; j++) {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }

    return result;
}

matrix mul_matrix(matrix a, matrix b) {
    int arows = a.get_rows();
    int bcols = b.get_cols();

    if (a.get_cols() != b.get_rows()) {
        std::cerr << "Matrix dimensions must match" << std::endl;
    }

    matrix result(arows, bcols);

    for (int i = 0; i < arows; i++) {
        for (int j = 0; j < bcols; j++) {
            for (int k = 0; k < a.get_cols(); k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

matrix add_matrix_vector(matrix mat, matrix vect) {
    int mat_rows = mat.get_rows();
    int mat_cols = mat.get_cols();

    int vect_rows = vect.get_rows();
    int vect_cols = vect.get_cols();

    if (vect_rows != 1 && vect_cols != 1) {
        std::cerr << "Vector must be a vector" << std::endl;
    }
    bool is_row = vect_rows == 1;
    if(mat_cols != vect_cols) {
        std::cerr << "Matrix and vector dimensions must match" << std::endl;
    }

    matrix result(mat_rows, mat_cols);

    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            result.data[i][j] = mat.data[i][j] + (is_row ? vect.data[0][j] : vect.data[j][0]);
        }
    }
    return result;

}

matrix mul_matrix_vector(matrix mat, matrix vect) {
    int mat_rows = mat.get_rows();
    int mat_cols = mat.get_cols();

    int vect_rows = vect.get_rows();
    int vect_cols = vect.get_cols();

    if (vect_rows != 1) {
        std::cerr << "Vector must be a row vector" << std::endl;
    }

    matrix result(mat_rows, vect_cols);

    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < vect_cols; j++) {
            for (int k = 0; k < mat_cols; k++) {
                result.data[i][j] += mat.data[i][k] * vect.data[0][j];
            }
        }
    }
    return result;

}

matrix transpose_matrix(matrix a) {
    int arows = a.get_rows();
    int acols = a.get_cols();

    matrix result(acols, arows);

    for (int i = 0; i < arows; i++) {
        for (int j = 0; j < acols; j++) {
            result.data[j][i] = a.data[i][j];
        }
    }
    return result;
}

matrix scalar_multiply(matrix mat, float scalar)
{
    int rows = mat.get_rows();
    int cols = mat.get_cols();

    matrix result(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.data[i][j] = mat.data[i][j] * scalar;
        }
    }
    return result;
}

matrix hadamard_product(matrix a, matrix b)
{
    int arows = a.get_rows();
    int acols = a.get_cols();

    if (arows != b.get_rows() || acols != b.get_cols()) {
        std::cerr << "Matrix dimensions must match" << std::endl;
    }

    matrix result(arows, acols);

    for (int i = 0; i < arows; i++) {
        for (int j = 0; j < acols; j++) {
            result.data[i][j] = a.data[i][j] * b.data[i][j];
        }
    }

    return result;
}

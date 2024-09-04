#include <activation.hpp>

matrix sigmoid(matrix* a) {
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = 1.0 / (1.0 + exp(-a->data[i][j]));
        }
    }
    return result;
}

matrix sigmoid(matrix a) {
    matrix result(a.get_rows(), a.get_cols());
    for (int i = 0; i < a.get_rows(); i++) {
        for (int j = 0; j < a.get_cols(); j++) {
            result.data[i][j] = 1.0 / (1.0 + exp(-a.data[i][j]));
        }
    }
    return result;
}

matrix sigmoid_prime(matrix* a) {
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] * (1.0 - a->data[i][j]);
        }
    }
    return result;
}

matrix relu(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] > 0 ? a->data[i][j] : 0;
        }
    }
    return result;
}

matrix relu(matrix a)
{
    matrix result(a.get_rows(), a.get_cols());
    for (int i = 0; i < a.get_rows(); i++) {
        for (int j = 0; j < a.get_cols(); j++) {
            result.data[i][j] = a.data[i][j] > 0 ? a.data[i][j] : 0;
        }
    }
    return result;
}

matrix relu_prime(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] > 0 ? 1 : 0;
        }
    }
    return result;
}


matrix leaky_relu(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] > 0 ? a->data[i][j] : 0.01 * a->data[i][j];
        }
    }
    return result;
}

matrix leaky_relu_prime(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] > 0 ? 1 : 0.01;
        }
    }
    return result;
}

matrix tanh(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = tanh(a->data[i][j]);
        }
    }
    return result;
}

matrix tanh_prime(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = 1 - pow(tanh(a->data[i][j]), 2);
        }
    }
    return result;
}

matrix softmax(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        float sum = 0;
        for (int j = 0; j < a->get_cols(); j++) {
            sum += exp(a->data[i][j]);
        }
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = exp(a->data[i][j]) / sum;
        }
    }
    return result;
}

matrix softmax_prime(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] * (1.0 - a->data[i][j]);
        }
    }
    return result;
}

matrix linear(matrix *a)
{
    return *a;
}

matrix linear_prime(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = 1;
        }
    }
    return result;
}

matrix binary_step(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = a->data[i][j] < 0 ? 0 : 1;
        }
    }
    return result;
}

matrix binary_step_prime(matrix *a)
{
    matrix result(a->get_rows(), a->get_cols());
    for (int i = 0; i < a->get_rows(); i++) {
        for (int j = 0; j < a->get_cols(); j++) {
            result.data[i][j] = 0;
        }
    }
    return result;
}
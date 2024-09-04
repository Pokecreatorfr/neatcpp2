#pragma once

#include <matrix.hpp>
#include <math.h>

matrix sigmoid(matrix* a);

matrix sigmoid(matrix a);

matrix sigmoid_prime(matrix* a);

matrix relu(matrix* a);

matrix relu(matrix a);

matrix relu_prime(matrix* a);

matrix leaky_relu(matrix* a);

matrix leaky_relu_prime(matrix* a);

matrix tanh(matrix* a);

matrix tanh_prime(matrix* a);

matrix softmax(matrix* a);

matrix softmax_prime(matrix* a);

matrix linear(matrix* a);

matrix linear_prime(matrix* a);

matrix binary_step(matrix* a);

matrix binary_step_prime(matrix* a);
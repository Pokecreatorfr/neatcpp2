#pragma once

#include <activation.hpp>

struct layer {
    matrix* weights;
    matrix* biases;
};



class Network
{
    public:
        Network(std::vector<int> topology);

        Network(std::vector<layer> layers);

        //destructor
        ~Network();

        void mutate(float weight_mutation_rate , float bias_mutation_rate , float add_neuron_rate , float add_layer_rate);

        Network* clone();

        matrix feed_forward(matrix input);

        float compute_loss(matrix& predicted,matrix& actual);

        void backpropagation(matrix* input, matrix* target, float learning_rate);

        void print_topology();

        void print_weights();

        void print_biases();

        void print_info();

    private:
        std::vector<layer> layers;
};
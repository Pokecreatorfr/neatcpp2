#include <network.hpp>
Network::Network(std::vector<int> topology) {
    for (int i = 0; i < topology.size(); i++) {
        int num_neurons = topology[i];
        int num_inputs = i == 0 ? 6 : topology[i - 1];
        layer l;
        l.weights = new matrix(num_neurons, num_inputs);
        l.biases = new matrix(num_neurons, 1);
        // randomize weights and biases between -1 and 1
        for (int j = 0; j < num_neurons; j++) {
            for (int k = 0; k < num_inputs; k++) {
                l.weights->data[j][k] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
            }
            l.biases->data[j][0] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
        }
        layers.push_back(l);
    }
}
Network::Network(std::vector<layer> layers)
{
    this->layers = layers;    
}
Network::~Network()
{
    for (int i = 0; i < layers.size(); i++) {
        delete layers[i].weights;
        delete layers[i].biases;
    }
}
void Network::mutate(float weight_mutation_rate, float bias_mutation_rate, float add_neuron_rate, float add_layer_rate)
{
    if ((float)rand() / RAND_MAX < add_layer_rate) {
        int num_neurons = 1;
        int num_inputs = layers.back().weights->get_rows();
        layer l;
        l.weights = new matrix(num_neurons, num_inputs);
        l.biases = new matrix(1, num_neurons);
        // randomize weights and biases between -1 and 1
        l.weights->data[0][0] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
        l.biases->data[0][0] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
        layers.push_back(l);
    }
    for (int i = 0; i < layers.size(); i++) {
        if((float)rand() / RAND_MAX < add_neuron_rate)
        {
            std::vector<float> new_weights;
            std::vector<float> new_biases;
            for (int j = 0; j < layers[i].weights->get_cols(); j++) {
                new_weights.push_back(2.0 * ((float)rand() / RAND_MAX) - 1.0);
            }
            new_biases.push_back(2.0 * ((float)rand() / RAND_MAX) - 1.0);
            layers[i].weights->add_row(new_weights);
            layers[i].biases->add_row(new_biases);

            if(i < layers.size() - 1)
            {
                std::vector<float> new_weights;
                int cols = layers[i + 1].weights->get_rows();
                for (int j = 0; j < cols; j++) {
                    new_weights.push_back(2.0 * ((float)rand() / RAND_MAX) - 1.0);
                }
                layers[i + 1].weights->add_col(new_weights);
            }

        }

        for (int j = 0; j < layers[i].weights->get_rows(); j++) {
            for (int k = 0; k < layers[i].weights->get_cols(); k++) {
                if ((float)rand() / RAND_MAX < weight_mutation_rate) {
                    layers[i].weights->data[j][k] += 2.0 * ((float)rand() / RAND_MAX) - 1.0;
                }
            }
            if ((float)rand() / RAND_MAX < bias_mutation_rate) {
                layers[i].biases->data[j][0] += 2.0 * ((float)rand() / RAND_MAX) - 1.0;
            }
        }
    }
}
Network* Network::clone() {
    std::vector<layer> layers;
    for (int i = 0; i < this->layers.size(); i++) {
        layer l;
        l.weights = this->layers[i].weights->clone();
        l.biases = this->layers[i].biases->clone();
        layers.push_back(l);
    }
    return new Network(layers);
}
matrix Network::feed_forward(matrix input) {
    matrix result = input;
    for (int i = 0; i < layers.size(); i++) {
        result = relu(add_matrix_vector( mul_matrix(*layers[i].weights, result) , *layers[i].biases));
        //std::cout << "Local Result: " << result.data[0][0] << std::endl;
        
    }
    //std::cout << "Result: " << result.data[0][0] << std::endl;
    return result;
}
float Network::compute_loss(matrix &predicted,matrix &actual)
{    

    if(predicted.get_rows() != actual.get_rows() || predicted.get_cols() != actual.get_cols())
    {
        std::cerr << "Matrix dimensions must match" << std::endl;
    }

    float error = 0;
    int rows = predicted.get_rows();
    int cols = predicted.get_cols();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            error += pow(predicted.data[i][j] - actual.data[i][j], 2);
        }
    }
    return error / (rows * cols);
}

void Network::backpropagation(matrix* input, matrix *target, float learning_rate)
{
    // Forward pass
    std::vector<matrix> activations;
    std::vector<matrix> z_values;
    matrix activation = *input;
    activations.push_back(activation);

    std::cout << "Input: " << input->data[0][0] << std::endl;
    std::cout << "Target: " << target->data[0][0] << std::endl;
    std::cout << "Feed Forward " << feed_forward(*input).data[0][0] << std::endl;

    for (int i = 0; i < layers.size(); i++) {
        matrix z = add_matrix_vector( mul_matrix(*layers[i].weights, activation) , *layers[i].biases);
        std::cout << "Z: " << z.data[0][0] << std::endl;
        z_values.push_back(z);

        activation = sigmoid(&z);  // Apply activation function
        activations.push_back(activation);
    }

    // print first layer activations
    //std::cout << "Input: " << input.data[0][0] << std::endl;
    //std::cout << "Output: " << activations.back().data[0][0] << std::endl;

    // Backward pass: calculate gradients
    matrix delta = sub_matrix(activations.back(), *target); // Initial delta for the output layer

    for (int i = layers.size() - 1; i >= 0; i--) {
        // Gradient for biases
        matrix bias_gradient = delta;
        matrix biases = sub_matrix(*layers[i].biases, scalar_multiply(bias_gradient, learning_rate));
        delete layers[i].biases;
        layers[i].biases = biases.clone();

        // Gradient for weights
        matrix weight_gradient = mul_matrix(delta, transpose_matrix(activations[i]));
        matrix weights = sub_matrix(*layers[i].weights, scalar_multiply(weight_gradient, learning_rate));
        delete layers[i].weights;
        layers[i].weights = weights.clone();

        if (i > 0) {
            // Calculate delta for the next layer
            delta = mul_matrix(transpose_matrix(*layers[i].weights), delta);
            delta = hadamard_product(delta, sigmoid_prime(&z_values[i - 1])); // Apply derivative of activation
        }
    }
}
void Network::print_topology()
{
    for (int i = 0; i < layers.size(); i++) {
        std::cout << "Layer: " << i << std::endl;
        std::cout << "Weights: " << layers[i].weights->get_rows() << "x" << layers[i].weights->get_cols() << std::endl;
        std::cout << "Biases: " << layers[i].biases->get_rows() << "x" << layers[i].biases->get_cols() << std::endl;
    }
}

void Network::print_weights()
{
    for (int i = 0; i < layers.size(); i++) {
        std::cout << "Layer: " << i << std::endl;
        std::cout << "Weights: " << std::endl;
        for (int j = 0; j < layers[i].weights->get_rows(); j++) {
            for (int k = 0; k < layers[i].weights->get_cols(); k++) {
                std::cout << layers[i].weights->data[j][k] << " ";
            }
            std::cout << std::endl;
        }
    }
}

void Network::print_biases()
{
    for (int i = 0; i < layers.size(); i++) {
        std::cout << "Layer: " << i << std::endl;
        std::cout << "Biases: " << std::endl;
        for (int j = 0; j < layers[i].biases->get_rows(); j++) {
            for (int k = 0; k < layers[i].biases->get_cols(); k++) {
                std::cout << layers[i].biases->data[j][k] << " ";
            }
            std::cout << std::endl;
        }
    }
}

void Network::print_info()
{
    std::cout << "Layer number:" << layers.size() << std::endl;
    int param_count = 0;
    for (int i = 0; i < layers.size(); i++) {
        param_count += layers[i].weights->get_rows() * layers[i].weights->get_cols();
        param_count += layers[i].biases->get_rows() * layers[i].biases->get_cols();
    }
    std::cout << "Parameter count:" << param_count << std::endl;
}

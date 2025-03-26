#include <iostream>
#include <vector>

// Header guard
#ifndef _NETWORK_H

// Settings
#define _NETWORK_ID
#define _NETWORK_LOG

// Array types and random function
typedef double _type;
#define _v(T) std::vector<T>
#define _v2(T) std::vector<std::vector<T>>
#define _v3(T) std::vector<std::vector<std::vector<T>>>
#define random(LOW, HIGH) (LOW + (float)(rand()) / ((float)(RAND_MAX / (HIGH - LOW))))

/// @brief Class for managing a single neural network
class neuralNetwork {
private:

    /// @brief Array containing the expected size of each input, output, and hidden layer.
    _v(int) layers;

    /// @brief Data object containing neuron/weight/bias values for the entire network
    struct networkData {
        _v2(_type) neurons;
        _v2(_type) biases;
        _v3(_type) weights;
    } data;

public:

    /// @brief Value representing network performance, should be determined after forwarding
    float fitness;

    /// @brief Initialization functions
    void createNeurons();
    void createBiases();
    void createWeights();

    /// @brief Feeds an input through the neural network and returns the network's output
    /// @param input The input array to forward through the neural network
    /// @return The output layer of the neural network
    _v(_type) forward(_v(_type) input);

    /// @brief Mutates the network by adding a randomized value to the weights/biases
    /// @param chance The chance from 0.f to 1.f for the network to mutate
    /// @param maxMutationAmount The maximum value that the weights/biases can deviate by
    void mutate(float chance, _type maxMutationAmount);

    /// @brief Copies the weights/biases from the Other network to the calling network.
    /// @param other A pointer to the Network object to clone data from
    void cloneFrom(neuralNetwork* other);

    /// @brief Converts network weights/biases into a string format
    /// @return A string containing serialized network data which can be saved to a file
    std::string serialize();

    /// @brief Loads string-formatted network data into the network weights/biases
    /// @param serialized The input string-formatted network data
    void load(std::string serialized);

    /// @brief Gets the number of values returned by serializing the network
    /// @return The number of serialized values
    unsigned long getSerializeCount();

    /// @brief Constructor used to specify layer sizes
    /// @param newLayers The layer sizes to use
    neuralNetwork(_v(int) newLayers);
};

// Header guard
#define _NETWORK_H
#endif
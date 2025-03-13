#include <iostream>
#include <vector>

// Header guard
#ifndef _NETWORK_H

// Settings
#define _NETWORK_ID
#define _NETWORK_LOG

// Array types and random function
#define _V(T) std::vector<T>
#define _V2(T) std::vector<std::vector<T>>
#define _V3(T) std::vector<std::vector<std::vector<T>>>
#define Random(LOW, HIGH) (LOW + (float)(rand()) / ((float)(RAND_MAX / (HIGH - LOW))))

/// @brief Class for managing a single neural network
class NeuralNetwork {
private:

    /// @brief Array containing the expected size of each input, output, and hidden layer.
    _V(int) Layers;

    /// @brief Data object containing neuron/weight/bias values for the entire network
    struct NetworkData {
        _V2(float) Neurons;
        _V2(float) Biases;
        _V3(float) Weights;
    } data;

public:

    /// @brief Value representing network performance, should be determined after forwarding
    float Fitness;

    /// @brief Initialization functions
    void CreateNeurons();
    void CreateBiases();
    void CreateWeights();

    /// @brief Feeds an input through the neural network and returns the network's output
    /// @param Input The input array to forward through the neural network
    /// @return The output layer of the neural network
    _V(float) Forward(_V(float) Input);

    /// @brief Mutates the network by adding a randomized value to the weights/biases
    /// @param Chance The chance from 0.f to 1.f for the network to mutate
    /// @param Value The maximum value that the weights/biases can deviate by
    void Mutate(float Chance, float Value);

    /// @brief Copies the weights/biases from the Other network to the calling network.
    /// @param Other A pointer to the Network object to clone data from
    void CloneFrom(NeuralNetwork* Other);

    /// @brief Compares the performance of the current network to another
    /// @param Other A pointer to the network object to compare to
    /// @return Returns true if the calling network has a higher fitness than the Other
    bool Compare(NeuralNetwork* Other);

    /// @brief Converts network weights/biases into a string format
    /// @return A string containing serialized network data which can be saved to a file
    std::string Serialize();

    /// @brief Loads string-formatted network data into the network weights/biases
    /// @param Serialized The input string-formatted network data
    void Load(std::string Serialized);

    /// @brief Constructor used to specify layer sizes
    /// @param NewLayers The layer sizes to use
    NeuralNetwork(_V(int) NewLayers);
};

// Header guard
#define _NETWORK_H
#endif
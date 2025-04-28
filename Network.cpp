#include "Network.h"

#include <sstream>
#include <cmath>

void neuralNetwork::createNeurons() {
    data.neurons.clear();
    for (unsigned long x = 0; x < layers.size(); x++) {
        _v(_type) newNeuronsArray;
        for (int y = 0; y < layers[x]; y++) {
            newNeuronsArray.push_back(0);
        }
        data.neurons.push_back(newNeuronsArray);
    }
}

void neuralNetwork::createBiases() {
    data.biases.clear();
    for (unsigned long x = 0; x < layers.size(); x++) {
        _v(_type) newBiasesArray;
        for (int y = 0; y < layers[x]; y++) {
            newBiasesArray.push_back(0);
        }
        data.biases.push_back(newBiasesArray);
    }
}

void neuralNetwork::createWeights() {
    data.weights.clear();
    for (unsigned long x = 1; x < layers.size(); x++) {
        _v2(_type) newWeightsArray;
        int neuronsInPreviousLayer = layers[x - 1];
        for (unsigned long y = 0; y < data.neurons[x].size(); y++) {
            _v(_type) newWeightsArrayArray;
            for (int z = 0; z < neuronsInPreviousLayer; z++) {
                newWeightsArrayArray.push_back(0);
            }
            newWeightsArray.push_back(newWeightsArrayArray);
        }
        data.weights.push_back(newWeightsArray);
    }
}

_v(_type) neuralNetwork::forward(_v(_type) input) {

    // Set the first layer of neurons to the input array
    for (unsigned long x = 0; x < input.size(); x++) {
        data.neurons[0][x] = input[x];
    }

    for (unsigned long x = 1; x < layers.size(); x++) {
        int workingLayer = x - 1;
        for (unsigned long y = 0; y < data.neurons[x].size(); y++) {

            _type newNeuronValue = 0.0f;
            for (unsigned long z = 0; z < data.neurons[workingLayer].size(); z++) {
                newNeuronValue += data.weights[workingLayer][y][z] * data.neurons[workingLayer][z];
            }

            data.neurons[x][y] = tanh(newNeuronValue + data.biases[x][y]);
        }
    }
    return data.neurons[data.neurons.size() - 1];
}

void neuralNetwork::mutate(_type maxMutationAmount) {
    for (unsigned long x = 0; x < data.biases.size(); x++) {
        for (unsigned long y = 0; y < data.biases[x].size(); y++) {
            data.biases[x][y] += random(-maxMutationAmount, maxMutationAmount);
        }
    }
    for (unsigned long x = 0; x < data.weights.size(); x++) {
        for (unsigned long y = 0; y < data.weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.weights[x][y].size(); z++) {
                data.weights[x][y][z] += random(-maxMutationAmount, maxMutationAmount);
            }
        }
    }
}

void neuralNetwork::cloneFrom(neuralNetwork* other) {
    data = other->data;
    fitness = other->fitness;
}

std::string neuralNetwork::serialize() {

    auto serializeType = [](_type t) -> std::string {
        char buf[128];
        snprintf(buf, sizeof(buf), "%1.32f", t);
        return std::string(buf);
        };

    std::stringstream stream;

    for (unsigned long x = 0; x < data.biases.size(); x++) {
        for (unsigned long y = 0; y < data.biases[x].size(); y++) {
            stream << serializeType(data.biases[x][y]) << "\n";
        }
    }
    for (unsigned long x = 0; x < data.weights.size(); x++) {
        for (unsigned long y = 0; y < data.weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.weights[x][y].size(); z++) {
                stream << serializeType(data.weights[x][y][z]) << "\n";
            }
        }
    }

    return stream.str();
}

void neuralNetwork::load(std::string serialized) {

    // Split into lines
    std::stringstream stream(serialized);
    std::string currentLine;
    _v(std::string) lines;
    while (std::getline(stream, currentLine)) {
        lines.push_back(currentLine);
    }

    // Parse lines
    unsigned long index = 1;
    for (unsigned long x = 0; x < data.biases.size(); x++) {
        for (unsigned long y = 0; y < data.biases[x].size(); y++) {
            data.biases[x][y] = (_type)std::stod(lines[index]);
            index++;
        }
    }
    for (unsigned long x = 0; x < data.weights.size(); x++) {
        for (unsigned long y = 0; y < data.weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.weights[x][y].size(); z++) {
                index++;
                if (lines.size() >= index + 1) {
                    data.weights[x][y][z] = (_type)std::stod(lines[index]);
                }
            }
        }
    }
}

unsigned long neuralNetwork::getSerializeCount() {
    unsigned long finalCount = 0;
    for (unsigned long x = 0; x < data.biases.size(); x++) {
        for (unsigned long y = 0; y < data.biases[x].size(); y++) {
            finalCount++;
        }
    }
    for (unsigned long x = 0; x < data.weights.size(); x++) {
        for (unsigned long y = 0; y < data.weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.weights[x][y].size(); z++) {
                finalCount++;
            }
        }
    }
    return finalCount;
}

neuralNetwork::neuralNetwork(_v(int) newLayers) {

    layers = newLayers;
    for (unsigned long i = 0; i < layers.size(); i++) {
        layers[i] = newLayers[i];
    }

    // Initialize network
    createNeurons();
    createBiases();
    createWeights();
}
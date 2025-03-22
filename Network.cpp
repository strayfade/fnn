#include "Network.h"

#include <sstream>
#include <cmath>

void NeuralNetwork::CreateNeurons() {
    data.Neurons.clear();
    for (unsigned long x = 0; x < Layers.size(); x++) {
        _V(float) NewNeuronsArray;
        for (int y = 0; y < Layers[x]; y++) {
            NewNeuronsArray.push_back(0);
        }
        data.Neurons.push_back(NewNeuronsArray);
    }
}

void NeuralNetwork::CreateBiases() {
    data.Biases.clear();
    for (unsigned long x = 0; x < Layers.size(); x++) {
        _V(float) NewBiasesArray;
        for (int y = 0; y < Layers[x]; y++) {
            NewBiasesArray.push_back(0);
        }
        data.Biases.push_back(NewBiasesArray);
    }
}

void NeuralNetwork::CreateWeights() {
    data.Weights.clear();
    for (unsigned long x = 1; x < Layers.size(); x++) {
        _V2(float) NewWeightsArray;
        int NeuronsInPreviousLayer = Layers[x - 1];
        for (unsigned long y = 0; y < data.Neurons[x].size(); y++) {
            _V(float) NewWeightsArrayArray;
            for (int z = 0; z < NeuronsInPreviousLayer; z++) {
                NewWeightsArrayArray.push_back(0);
            }
            NewWeightsArray.push_back(NewWeightsArrayArray);
        }
        data.Weights.push_back(NewWeightsArray);
    }
}

_V(float) NeuralNetwork::Forward(_V(float) Input) {
    for (unsigned long x = 0; x < Input.size(); x++) {
        data.Neurons[0][x] = Input[x];
    }
    for (unsigned long x = 1; x < Layers.size(); x++) {
        int WorkingLayer = x - 1;
        for (unsigned long y = 0; y < data.Neurons[x].size(); y++) {
            float Value = 0.0f;
            for (unsigned long z = 0; z < data.Neurons[WorkingLayer].size(); z++) {
                Value += data.Weights[WorkingLayer][y][z] * data.Neurons[WorkingLayer][z];
            }
            data.Neurons[x][y] = tanh(Value + data.Biases[x][y]);
        }
    }
    return data.Neurons[data.Neurons.size() - 1];
}

void NeuralNetwork::Mutate(float Chance = 0.5f, float Value = 0.5f) {
    for (unsigned long x = 0; x < data.Biases.size(); x++) {
        for (unsigned long y = 0; y < data.Biases[x].size(); y++) {
            if (Random(0.0f, 1.0f) <= Chance) {
                data.Biases[x][y] += Random(-Value, Value);
            }
        }
    }
    for (unsigned long x = 0; x < data.Weights.size(); x++) {
        for (unsigned long y = 0; y < data.Weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.Weights[x][y].size(); z++) {
                if (Random(0.0f, Chance) <= 0.5) {
                    data.Weights[x][y][z] += Random(-Value, Value);
                }
            }
        }
    }
}

void NeuralNetwork::CloneFrom(NeuralNetwork* other) {
    data = other->data;
    Fitness = other->Fitness;
}

bool NeuralNetwork::Compare(NeuralNetwork* other) {
    return other->Fitness < Fitness;
}

std::string NeuralNetwork::Serialize() {
    std::stringstream stream;
    
    for (unsigned long x = 0; x < data.Biases.size(); x++) {
        for (unsigned long y = 0; y < data.Biases[x].size(); y++) {
            stream << std::to_string(data.Biases[x][y]) << "\n";
        }
    }
    for (unsigned long x = 0; x < data.Weights.size(); x++) {
        for (unsigned long y = 0; y < data.Weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.Weights[x][y].size(); z++) {
                stream << std::to_string(data.Weights[x][y][z]) << "\n";
            }
        }
    }
    
    return stream.str();
}

void NeuralNetwork::Load(std::string Serialized) {

    // Split into lines
    std::stringstream stream(Serialized);
    std::string currentLine;
    _V(std::string) lines;
    while (std::getline(stream, currentLine)) {
        lines.push_back(currentLine);
    }

    // Parse lines
    unsigned long index = 1;
    for (unsigned long x = 0; x < data.Biases.size(); x++) {
        for (unsigned long y = 0; y < data.Biases[x].size(); y++) {
            data.Biases[x][y] = std::stof(lines[index]);
            index++;
        }
    }
    for (unsigned long x = 0; x < data.Weights.size(); x++) {
        for (unsigned long y = 0; y < data.Weights[x].size(); y++) {
            for (unsigned long z = 0; z < data.Weights[x][y].size(); z++) {
                index++;
                if (lines.size() >= index + 1) {
                    data.Weights[x][y][z] = std::stof(lines[index]);
                }
            }
        }
    }
}

NeuralNetwork::NeuralNetwork(_V(int) NewLayers) {

    Layers = NewLayers;
    for (unsigned long i = 0; i < Layers.size(); i++) {
        Layers[i] = NewLayers[i];
    }

    // Initialize network
    CreateNeurons();
    CreateBiases();
    CreateWeights();
}
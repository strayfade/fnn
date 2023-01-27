#include "Network.h"

#include <iostream>

#include <sstream>
#include <fstream>
#include <sstream>
#include <istream>
#include <ostream>

void NeuralNetwork::CreateNeurons() {
    V2(float) NewNeurons;
    for (int x = 0; x < this->Layers.size(); x++) {
        V(float) NewNeuronsArray;
        for (int y = 0; y < this->Layers[x]; y++) {
            NewNeuronsArray.push_back(0);
        }
        NewNeurons.push_back(NewNeuronsArray);
    }
    this->Neurons = NewNeurons;
}
void NeuralNetwork::CreateBiases() {
    V2(float) NewBiases;
    for (int x = 0; x < this->Layers.size(); x++) {
        V(float) NewBiasesArray;
        for (int y = 0; y < this->Layers[x]; y++) {
            NewBiasesArray.push_back(Random(-0.5f, 0.5f));
        }
        NewBiases.push_back(NewBiasesArray);
    }
    this->Biases = NewBiases;
}
void NeuralNetwork::CreateWeights() {
    V3(float) NewWeights;
    for (int x = 1; x < this->Layers.size(); x++) {
        V2(float) NewWeightsArray;
        int NeuronsInPreviousLayer = this->Layers[x - 1];
        for (int y = 0; y < this->Neurons[x].size(); y++) {
            V(float) NewWeightsArrayArray;
            for (int z = 0; z < NeuronsInPreviousLayer; z++) {
                NewWeightsArrayArray.push_back(Random(-0.5f, 0.5f));
            }
            NewWeightsArray.push_back(NewWeightsArrayArray);
        }
        NewWeights.push_back(NewWeightsArray);
    }
    this->Weights = NewWeights;
}

V(float) NeuralNetwork::Forward(V(float) Input) {
    for (int x = 0; x < Input.size(); x++) {
        this->Neurons[0][x] = Input[x];
    }
    for (int x = 1; x < this->Layers.size(); x++) {
        int WorkingLayer = x - 1;
        for (int y = 0; y < this->Neurons[x].size(); y++) {
            float Value = 0.0f;
            for (int z = 0; z < this->Neurons[WorkingLayer].size(); z++) {
                Value += this->Weights[WorkingLayer][y][z] * this->Neurons[WorkingLayer][z];
            }
            this->Neurons[x][y] = tanh(Value + this->Biases[x][y]);
        }
    }
    return this->Neurons[Neurons.size() - 1];
}
void NeuralNetwork::Mutate(int Chance, float Value) {
    for (int x = 0; x < this->Biases.size(); x++) {
        for (int y = 0; y < this->Biases[x].size(); y++) {
            if (Random(0.0f, Chance) <= 0.5) {
                this->Biases[x][y] += Random(-Value, Value);
            }
        }
    }
    for (int x = 0; x < this->Weights.size(); x++) {
        for (int y = 0; y < this->Weights[x].size(); y++) {
            for (int z = 0; z < this->Weights[x][y].size(); z++) {
                if (Random(0.0f, Chance) <= 0.5) {
                    this->Weights[x][y][z] += Random(-Value, Value);
                }
            }
        }
    }
}

void NeuralNetwork::CloneFrom(NeuralNetwork Other) {
    this->Biases = Other.Biases;
    this->Weights = Other.Weights;
    this->Fitness = Other.Fitness;
}

NeuralNetwork::ComparisonResults NeuralNetwork::CompareTo(NeuralNetwork Other) {
    if (Other.Fitness < this->Fitness)
        return NeuralNetwork::ComparisonResults::Worse;
    else if (Other.Fitness > this->Fitness)
        return NeuralNetwork::ComparisonResults::Better;
    return NeuralNetwork::ComparisonResults::Equal;
}

bool NeuralNetwork::Save(std::string Path) {
    std::ofstream File(Path);
    if (!File.is_open())
        return false;
    for (int x = 0; x < this->Biases.size(); x++) {
        for (int y = 0; y < this->Biases[x].size(); y++) {
            File << std::to_string(this->Biases[x][y]) << "\n";
        }
    }
    for (int x = 0; x < this->Weights.size(); x++) {
        for (int y = 0; y < this->Weights[x].size(); y++) {
            for (int z = 0; z < this->Weights[x][y].size(); z++) {
                File << std::to_string(this->Weights[x][y][z]) << "\n";
            }
        }
    }
    File.close();
    return true;
}
bool NeuralNetwork::Load(std::string Path) {
    std::ifstream File(Path);
    if (!File.is_open())
        return false;

    // Read all lines from file
    V(std::string) Lines;
    std::string CurrentLine;
    while (getline(File, CurrentLine)) {
        Lines.push_back(CurrentLine);
    }
    File.close();

    int Index = 1;
    for (int x = 0; x < this->Biases.size(); x++) {
        for (int y = 0; y < this->Biases[x].size(); y++) {
            Biases[x][y] = std::stof(Lines[Index]);
            Index++;
        }
    }
    for (int x = 0; x < this->Weights.size(); x++) {
        for (int y = 0; y < this->Weights[x].size(); y++) {
            for (int z = 0; z < this->Weights[x][y].size(); z++) {
                Index++;
                if (Lines.size() >= Index + 1) {
                    this->Weights[x][y][z] = std::stof(Lines[Index]);
                }
            }
        }
    }
    return true;
}

NeuralNetwork::NeuralNetwork(V(int) NewLayers, bool CreateID) {

#ifdef _NETWORK_ID
    if (CreateID) {
        this->Id = _nid::CurrentId;
        _nid::CurrentId += 1;
    }
#endif

    this->Layers = NewLayers;
    for (int i = 0; i < this->Layers.size(); i++) {
        this->Layers[i] = NewLayers[i];
    }

    CreateNeurons();
    CreateBiases();
    CreateWeights();

    if (CreateID) {
        _L("Initialized network", false);
#ifdef _NETWORK_ID
        _L(" with ID " + std::to_string(this->Id), false);
#endif
        _L("!", true);
    }
}
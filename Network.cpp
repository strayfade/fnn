#include "Network.h"

void NeuralNetwork::CreateNeurons() {
    List<List<float>> NewNeurons;
    for (int i = 0; i < this->Layers.size(); i++) {
        List<float> NewList;
        NewNeurons.push_back(NewList);
    }
    this->Neurons = NewNeurons;  
}
void NeuralNetwork::CreateBiases() {
    List<List<float>> NewBiases;
    for (int i = 0; i < this->Layers.size(); i++) {
        List<float> Bias;
        for (int j = 0; j < this->Layers[i]; j++) {
            Bias.push_back(Random);
        }
        NewBiases.push_back(Bias);
    }
    this->Biases = NewBiases;
}
void NeuralNetwork::CreateWeights() {
    List<List<List<float>>> NewWeights;
    for (int i = 1; i < this->Layers.size(); i++) {
        List<List<float>> NewWeightsList;
        int NeuronsInPreviousLayer = Layers[i - 1];
        for (int x = 0; x < Neurons[i].size(); x++) {
            List<float> NewWeightsArray;
            for (int y = 0; y < NeuronsInPreviousLayer; y++) {
                NewWeightsArray.push_back(Random);
            }
            NewWeightsList.push_back(NewWeightsArray);
        }
        NewWeights.push_back(NewWeightsList);
    }
    this->Weights = NewWeights;
}

List<float> NeuralNetwork::Forward(List<float> Input) {
    for (int i = 0; i < Input.size(); i++) {
        this->Neurons[0][i] = Input[i];
    }
    for (int i = 1; i < Input.size(); i++) {
        int Layer = i - 1;
        for (int x = 0; x < this->Neurons[i].size(); x++) {
            float Val = 0.0f;
            for (int y = 0; y < this->Neurons[i - 1].size(); y++) {
                Val += this->Weights[i - 1][x][y] * this->Neurons[i - 1][y];
            }
            this->Neurons[i][x] = (float)tanh(Val + this->Biases[i][x]);
        }
    }
    return this->Neurons[this->Neurons.size() - 1];
}

// [Layers] Constructor
NeuralNetwork::NeuralNetwork(List<int> Layers) {
    for (int i = 0; i < Layers.size(); i++) {
        this->Layers[i] = Layers[i];
    }
    CreateNeurons();
    CreateBiases();
    CreateWeights();
}
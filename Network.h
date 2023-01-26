#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <string>

using namespace std;
#define V vector
#define Random(LO, HI) (LO + (float)(rand()) / ((float)(RAND_MAX/(HI - LO))))

class NeuralNetwork {
private:
    V<int> Layers;
    V<V<float>> Neurons;
    V<V<float>> Biases;
    V<V<V<float>>> Weights;
    V<int> Activations;
public:
    enum class ComparisonResults {
        Worse,
        Better,
        Equal
    };

    float Fitness = 0.0f;

    void CreateNeurons();
    void CreateBiases();
    void CreateWeights();

    V<float> Forward(V<float> Input);
    void Mutate(int Chance, float Value);
    void CloneFrom(NeuralNetwork Other);

    ComparisonResults CompareTo(NeuralNetwork Other);
    bool Save(string Path);
    bool Load(string Path);

    NeuralNetwork(V<int> NewLayers);
};

#include <iostream>
#include <vector>
#include <math.h>

#define List std::vector
#define Random ((float)(rand() / (float)RAND_MAX) - 0.5f)

class NeuralNetwork {
public:
    List<int> Layers;
    List<List<float>> Neurons;
    List<List<float>> Biases;
    List<List<List<float>>> Weights;
    List<int> Activations;

    void CreateNeurons();
    void CreateBiases();
    void CreateWeights();

    float Performance = 0.0f;
    NeuralNetwork(List<int> Layers);

    List<float> Forward(List<float> Input);
}
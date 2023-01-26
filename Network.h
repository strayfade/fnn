#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#ifndef _NETWORK_H

// Build Settings
#define _NETWORK_ID
#define _NETWORK_LOG

#ifdef _WIN32
#define V(T) std::vector<T>
#define V2(T) std::vector<std::vector<T>>
#define V3(T) std::vector<std::vector<std::vector<T>>>
#else
#define V(T) std::vector<T>
#define V2(T) std::vector<std::vector<T> >
#define V3(T) std::vector<std::vector<std::vector<T> > >
#endif
#define Random(LO, HI) (LO + (float)(rand()) / ((float)(RAND_MAX/(HI - LO))))

#ifdef _NETWORK_ID
namespace _nid {
    int CurrentId = 1;
}
#endif

#ifdef _NETWORK_LOG
#define _L(D, N) std::cout << D << (N ? "\n" : "")
#else
#define _L(D, N) 
#endif

class NeuralNetwork {
private:
    V(int) Layers;
    V2(float) Neurons;
    V2(float) Biases;
    V3(float) Weights;
public:
#ifdef _WIN32
    enum class ComparisonResults {
#else
    enum ComparisonResults {
#endif
        Worse,
        Better,
        Equal
    };

    #ifdef _NETWORK_ID
    int Id;
    #endif

    float Fitness;

    void CreateNeurons();
    void CreateBiases();
    void CreateWeights();

    V(float) Forward(V(float) Input);
    void Mutate(int Chance, float Value);
    void CloneFrom(NeuralNetwork Other);

    ComparisonResults CompareTo(NeuralNetwork Other);
    bool Save(std::string Path);
    bool Load(std::string Path);

    NeuralNetwork(V(int) NewLayers);
};
#define _NETWORK_H
#endif
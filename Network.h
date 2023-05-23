#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#ifndef _NETWORK_H

// Build Settings
#define _NETWORK_ID
#define _NETWORK_LOG

#ifdef _WIN32
#define _V(T) std::vector<T>
#define _V2(T) std::vector<std::vector<T>>
#define _V3(T) std::vector<std::vector<std::vector<T>>>
#else
#define _V(T) std::vector<T>
#define _V2(T) std::vector<std::vector<T> >
#define _V3(T) std::vector<std::vector<std::vector<T> > >
#endif

#define Random(LO, HI) (LO + (float)(rand()) / ((float)(RAND_MAX/(HI - LO))))

#ifdef _NETWORK_ID
namespace _nid {
    inline int CurrentId = 1;
}
#endif

#ifdef _NETWORK_LOG
#define _L(D, N) std::cout << D << (N ? "\n" : "")
#else
#define _L(D, N) 
#endif

class NeuralNetwork {
private:
    _V(int) Layers;
    _V2(float) Neurons;
    _V2(float) Biases;
    _V3(float) Weights;
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

    _V(float) Forward(_V(float) Input);
    void Mutate(float Chance, float Value);
    void CloneFrom(NeuralNetwork Other);

    ComparisonResults CompareTo(NeuralNetwork Other);
    bool Save(std::string Path);
    bool Load(std::string Path);

    NeuralNetwork(_V(int) NewLayers, bool CreateID = true);
};
#define _NETWORK_H
#endif
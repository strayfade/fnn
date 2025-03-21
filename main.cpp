#include "Pong.h"
#include "Network.h"

#include <vector>
#include <algorithm>

#define NETWORKS_LEN 50
#define NETWORKS_HIDDEN 20
#define NETWORKS_LAYERS { 2, NETWORKS_HIDDEN, 1 }

int main(void) {

    // Create network array
    std::vector<NeuralNetwork> networks;
    for (int i = 0; i < NETWORKS_LEN; i++) {
        networks.push_back(NeuralNetwork(NETWORKS_LAYERS));
    }
    
    // Here, we save the network with the highest performance each generation so that the next generation can use it
    NeuralNetwork highestPerformingNetwork(NETWORKS_LAYERS);

    // Variable to keep track of the generation count
    unsigned long iterations = 0;

    // Infinite training loop
    while (true) {

        iterations++;

        // Loop through all of the networks
        for (int x = 0; x < networks.size(); x++) {

            // Start by cloning each network from the best performing network
            networks[x].CloneFrom(&highestPerformingNetwork);

            // Slightly mutate networks to represent genetic mutation between generations
            networks[x].Mutate(0.5f, 1.0f);

            // Spin up a game of pong
            pongGame currentGame;

            // Run game until AI loses or until a time limit is hit
            unsigned long ticks = 0;
            while (!currentGame.gameOver && ticks < 999999) {

                // Get an array containing the ball X and Y, and the AI's current paddle position
                std::vector<float> inputs = currentGame.getNetworkInputs();

                // Forward the inputs through the network, returning an output array
                std::vector<float> outputs = networks[x].Forward(inputs);

                // Set the AI's paddle position and tick the Pong simulation
                currentGame.setAiPaddle(outputs[0]);
                currentGame.tick();
                ticks++;

                // Set the network's Fitness (scorekeeper) to the current number of ticks simulated
                networks[x].Fitness = ticks;
            }

            printf("Network %d finished training with score %d\n", x, ticks);
        }

        // Sort networks by highest performing to lowest
        std::sort(networks.begin(), networks.end(), [](const NeuralNetwork& a, const NeuralNetwork& b) {
            return a.Fitness > b.Fitness;
        });

        // Check if one of the newly trained networks is better than the highest performing network
        if (networks[0].Fitness > highestPerformingNetwork.Fitness) {

            // Clone the highest performing network for the next generation
            highestPerformingNetwork.CloneFrom(&networks[0]);
        }
        
        // Show demo game using the current highest performing network
        pongGame currentGame;
        while (!currentGame.gameOver) {
            std::vector<float> outputs = highestPerformingNetwork.Forward(currentGame.getNetworkInputs());
            currentGame.setAiPaddle(outputs[0]);
            currentGame.tick();
            currentGame.render();
        }

    }
}
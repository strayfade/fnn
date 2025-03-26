#include "Pong.h"
#include "Network.h"

#include <vector>
#include <algorithm>

#define NETWORKS_LEN 100
#define NETWORKS_HIDDEN 20, 20, 20
#define NETWORKS_LAYERS { 3, NETWORKS_HIDDEN, 1 }

int main() {

    // Create network array
    std::vector<neuralNetwork*> networks;
    for (int i = 0; i < NETWORKS_LEN; i++) {
        networks.push_back(new neuralNetwork(NETWORKS_LAYERS));
    }
    
    // Here, we save the network with the highest performance each generation so that the next generation can use it
    neuralNetwork highestPerformingNetwork(NETWORKS_LAYERS);

    // Infinite training loop
    while (true) {

        // Loop through all of the networks
        for (unsigned long x = 0; x < networks.size(); x++) {

            // Start by cloning each network from the best performing network
            networks[x]->cloneFrom(&highestPerformingNetwork);

            // Slightly mutate networks to represent genetic mutation between generations
            networks[x]->mutate(0.5f, 0.5f);

            // Spin up a game of pong
            pongGame currentGame;

            // Run game until AI loses or until a time limit is hit
            unsigned long ticks = 0;
            unsigned long numMoves = 0;
            int lastPosition = 0;
            while (!currentGame.gameOver && ticks < 999999) {

                // Get an array containing the ball X and Y, and the AI's current paddle position
                std::vector<_type> inputs = {
                    ((_type)currentGame.ball.y / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.ball.x / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.aiPaddlePosition / currentGame.getRendererSize().ws_row) * 2.f - 1.f
                };

                // Forward the inputs through the network, returning an output array
                std::vector<_type> outputs = networks[x]->forward(inputs);

                // Set the AI's paddle position and tick the Pong simulation
                currentGame.setAiPaddle(outputs[0]);

                if (currentGame.aiPaddlePosition != lastPosition) {
                    lastPosition = currentGame.aiPaddlePosition;
                    numMoves += abs(currentGame.aiPaddlePosition - lastPosition);
                }
                currentGame.tick();
                ticks++;

                // Set the network's Fitness (scorekeeper) to the current number of ticks simulated
                networks[x]->fitness = ticks - (float)numMoves * 3.f;

                if (ticks > 999990) {
                    ticks = 0;
                    networks[x]->fitness = 0;
                    break;
                }
            }

            //printf("Network %d finished training with score %d\n", x, ticks);
        }

        // Sort networks by highest performing to lowest
        std::sort(networks.begin(), networks.end(), [](const neuralNetwork* a, const neuralNetwork* b) {
            return a->fitness > b->fitness;
        });

        // Check if one of the newly trained networks is better than the highest performing network
        if (networks[0]->fitness > highestPerformingNetwork.fitness) {

            // Clone the highest performing network for the next generation
            highestPerformingNetwork.cloneFrom(networks[0]);
            
            printf("New local maximum: %5.2f\n", networks[0]->fitness);
        }

        if (highestPerformingNetwork.fitness > 2000) {

            //std::cout << highestPerformingNetwork.serialize() << std::endl;
            //std::cout << "\nmodifiable value count in system: " << (highestPerformingNetwork.getSerializeCount() * NETWORKS_LEN) << std::endl;
        
            // Show demo game using the current highest performing network
            pongGame currentGame;
            while (!currentGame.gameOver) {
                std::vector<_type> inputs = {
                    ((_type)currentGame.ball.y / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.ball.x / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.aiPaddlePosition / currentGame.getRendererSize().ws_row) * 2.f - 1.f
                };
                std::vector<_type> outputs = highestPerformingNetwork.forward(inputs);
                currentGame.setAiPaddle(outputs[0]);
                currentGame.tick();
                currentGame.render();
            }
            printf("\n\nBest network:\n%s\n\n", highestPerformingNetwork.serialize().c_str());

        }

    }
}
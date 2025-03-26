#include "Pong.h"
#include "Network.h"

#include <vector>

#define NETWORKS_LEN 50
#define NETWORKS_HIDDEN 20
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

            // Start by cloning each network from the best performing network (this is blank at first)
            networks[x]->cloneFrom(&highestPerformingNetwork);

            // Slightly mutate networks to represent natural random change between generations
            networks[x]->mutate();

            // Spin up a game of pong
            pongGame currentGame;

            // Run game until AI loses or until a time limit is hit
            unsigned long ticks = 0;
            unsigned long numMoves = 0;
            int lastPaddlePosition = 0;
            while (!currentGame.gameOver && ticks < 99999) {

                // Get an array containing the ball X and Y, and the AI's current paddle position
                // as input for the neural network. 
                // Importantly, all values are converted into a range from -1 to 1
                std::vector<_type> inputs = {
                    ((_type)currentGame.ball.y / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.ball.x / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.aiPaddlePosition / currentGame.getRendererSize().ws_row) * 2.f - 1.f
                };

                // Forward the inputs through the network, returning an output array
                std::vector<_type> outputs = networks[x]->forward(inputs);

                // Set the AI's paddle position
                currentGame.setAiPaddle(outputs[0]);

                // Calculates the distance the paddle moved so that it can be subtracted from the 
                // fitness score, creating a network that prefers as little movement as possible
                if (currentGame.aiPaddlePosition != lastPaddlePosition) {
                    lastPaddlePosition = currentGame.aiPaddlePosition;
                    numMoves += abs(currentGame.aiPaddlePosition - lastPaddlePosition);
                }

                // Tick the pongGame
                currentGame.tick();
                ticks++;

                // Set the network's Fitness (scorekeeper) to the current number of ticks simulated
                networks[x]->fitness = ticks - (float)numMoves * 3.f;

                // Failsafe in case the pongGame crashes or exceeds a time limit
                if (ticks > 99990) {
                    ticks = 0;
                    networks[x]->fitness = 0;
                    break;
                }
            }
            
            // Check if the current network performed better than best network
            if (networks[x]->fitness > highestPerformingNetwork.fitness) {

                // Set the "highest performing" network to the new best
                highestPerformingNetwork.cloneFrom(networks[x]);

                printf("New highest fitness: %5.2f\n", networks[x]->fitness);

                // Stop training and start new generation from new best network
                break;
            }

        }

        // If the best network is especially performant, render a sample game on-screen
        if (highestPerformingNetwork.fitness > 2000) {
        
            // Create a pongGame and run until the game ends
            pongGame currentGame;
            while (!currentGame.gameOver) {

                // Get network inputs
                std::vector<_type> inputs = {
                    ((_type)currentGame.ball.y / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.ball.x / currentGame.getRendererSize().ws_row) * 2.f - 1.f,
                    ((_type)currentGame.aiPaddlePosition / currentGame.getRendererSize().ws_row) * 2.f - 1.f
                };

                // Run the neural network
                std::vector<_type> outputs = highestPerformingNetwork.forward(inputs);

                // Set the paddle
                currentGame.setAiPaddle(outputs[0]);

                // Tick and render the pongGame
                currentGame.tick();
                currentGame.render();
            }

        }

    }
}
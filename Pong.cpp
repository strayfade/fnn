#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <cmath>

#include "Pong.h"

#define rng(LOW, HIGH) (LOW + (float)(rand()) / ((float)(RAND_MAX / (HIGH - LOW))))


#ifndef _WIN32
pongGame::rendererSize_t pongGame::getRendererSize() {
    if (cachedWindowSize.ws_col == 0) {
        winsize windowSize;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);
        cachedWindowSize.ws_col = windowSize.ws_col;
        cachedWindowSize.ws_row = windowSize.ws_row;
    }
    return cachedWindowSize;
}
#else
#endif

void setPixel(int x, int y) {
    printf("\033[%d;%dH", y, x);
    printf("%c", '#');
    fflush(stdout);
}

void pongGame::tick() {

    // Set both paddles' heights
    if (paddleHeight == -1) {
        paddleHeight = ceil(getRendererSize().ws_row / 4);
        aiPaddlePosition = floor(getRendererSize().ws_row / 2);
    }
    
    // Set ball position
    if (ball.x == -1) {
        ball.x = getRendererSize().ws_col / 2;
        ball.y = rng(1, getRendererSize().ws_row);
    }

    // Move ball 
    switch(ball.direction) {
        case ballDirection::TOP_LEFT:
            ball.x--;
            ball.y--;
        break;
        case ballDirection::TOP_RIGHT:
            ball.x++;
            ball.y--;
        break;
        case ballDirection::BOTTOM_LEFT:
            ball.x--;
            ball.y++;
        break;
        case ballDirection::BOTTOM_RIGHT:
            ball.x++;
            ball.y++;
        break;
    }

    // Ball collision with top and bottom walls
    if (ball.y >= getRendererSize().ws_row - 1) {
        ball.direction = ball.direction == ballDirection::BOTTOM_LEFT ? ballDirection::TOP_LEFT : ballDirection::TOP_RIGHT;
    }
    if (ball.y <= 2) {
        ball.direction = ball.direction == ballDirection::TOP_LEFT ? ballDirection::BOTTOM_LEFT : ballDirection::BOTTOM_RIGHT;
    }

    // Set perfect paddle position
    //perfectPaddlePosition = ball.y;

    // Collision with perfect paddle (right)
    if (ball.x >= getRendererSize().ws_col - 2) {
        if (ball.y > perfectPaddlePosition - paddleHeight / 2 - 1 && ball.y < perfectPaddlePosition + paddleHeight / 2 + 1) {
            ball.direction = ball.direction == ballDirection::TOP_RIGHT ? ballDirection::TOP_LEFT : ballDirection::BOTTOM_LEFT;
            opponentScore++;
        }
        else {
            gameOver = true;
        }
    }

    // Collision with AI paddle (left)
    if (ball.x == 3) {
        if (ball.y > aiPaddlePosition - paddleHeight / 2 - 1 && ball.y < aiPaddlePosition + paddleHeight / 2 + 1) {
            score++;
            ball.direction = ball.direction == ballDirection::TOP_LEFT ? ballDirection::TOP_RIGHT : ballDirection::BOTTOM_RIGHT;
        }
        else {
            gameOver = true;
        }
    }
}

void pongGame::render() {

    // Clear
    for (int i = 0; i < getRendererSize().ws_row; i++) {
        std::cout << "\n";
    }

    // Render borders
    /*for (int x = 0; x < getRendererSize().ws_col + 1; x++) {
        for (int y = 0; y < getRendererSize().ws_row + 1; y++) {
            if (x == 0 || y == 0 || x == getRendererSize().ws_col || y == getRendererSize().ws_row) {
                setPixel(x, y);
            }
        }
    }*/

    // Render ball
    setPixel(ball.x, ball.y);

    // Render scores
    printf("\033[%d;%dH", 2, 4);
    printf("%d", score);
    printf("\033[%d;%dH", 2, (int)(getRendererSize().ws_col - 2 - std::to_string(opponentScore).length()));
    printf("%d", opponentScore);

    // Render perfect paddle
    for (int i = perfectPaddlePosition - paddleHeight / 2; i < perfectPaddlePosition + paddleHeight / 2; i++) {
        if (i <= 0) continue;
        if (i >= getRendererSize().ws_row) continue;
        setPixel(getRendererSize().ws_col - 1, i);
    }

    // Render AI paddle
    for (int i = aiPaddlePosition - paddleHeight / 2; i < aiPaddlePosition + paddleHeight / 2; i++) {
        if (i <= 0) continue;
        if (i >= getRendererSize().ws_row) continue;
        setPixel(2, i);
    }
    
    // Move to end
    printf("\033[%d;%dH", getRendererSize().ws_row, getRendererSize().ws_col);
    printf("%c", '#');

    // Sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void pongGame::setAiPaddle(float y) {
    aiPaddlePosition = floor((y + 1) * getRendererSize().ws_row / 2);
}
void pongGame::setOpponentPaddle(float y) {
    perfectPaddlePosition = floor((y + 1) * getRendererSize().ws_row / 2);
}
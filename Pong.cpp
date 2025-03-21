#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "Pong.h"

#define rng(LOW, HIGH) (LOW + (float)(rand()) / ((float)(RAND_MAX / (HIGH - LOW))))

winsize getRendererSize() {
    winsize windowSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);
    return windowSize;
}

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
    if (ball.y >= getRendererSize().ws_row) {
        ball.direction = ball.direction == ballDirection::BOTTOM_LEFT ? ballDirection::TOP_LEFT : ballDirection::TOP_RIGHT;
    }
    if (ball.y <= 1) {
        ball.direction = ball.direction == ballDirection::TOP_LEFT ? ballDirection::BOTTOM_LEFT : ballDirection::BOTTOM_RIGHT;
    }

    // Set perfect paddle position
    perfectPaddlePosition = ball.y;

    // Collision with perfect paddle (right)
    if (ball.x >= getRendererSize().ws_col - 1) {
        ball.direction = ball.direction == ballDirection::TOP_RIGHT ? ballDirection::TOP_LEFT : ballDirection::BOTTOM_LEFT;
        opponentScore++;
    }

    // Collision with AI paddle (left)
    if (ball.x == 2) {
        if (ball.y > aiPaddlePosition - paddleHeight / 2 - 1 && ball.y < aiPaddlePosition + paddleHeight / 2 + 1) {
            score++;
            ball.direction = ball.direction == ballDirection::TOP_LEFT ? ballDirection::TOP_RIGHT : ballDirection::BOTTOM_RIGHT;
        }
        else {
            gameOver = true;
            score = 0;
            opponentScore = 0;
        }
    }
}

void pongGame::render() {

    // Clear
    for (int i = 0; i < getRendererSize().ws_row; i++) {
        std::cout << "\n";
    }

    // Render ball
    setPixel(ball.x, ball.y);

    // Render scores
    printf("\033[%d;%dH", 2, 4);
    printf("%d", score);
    printf("\033[%d;%dH", 2, getRendererSize().ws_col - 3);
    printf("%d", opponentScore);

    // Render perfect paddle
    for (int i = perfectPaddlePosition - paddleHeight / 2; i < perfectPaddlePosition + paddleHeight / 2; i++) {
        if (i <= 0) continue;
        if (i >= getRendererSize().ws_row) continue;
        setPixel(getRendererSize().ws_col, i);
    }

    // Render AI paddle
    for (int i = aiPaddlePosition - paddleHeight / 2; i < aiPaddlePosition + paddleHeight / 2; i++) {
        if (i <= 0) continue;
        if (i >= getRendererSize().ws_row) continue;
        setPixel(1, i);
    }
    
    // Move to end
    printf("\033[%d;%dH", getRendererSize().ws_row, getRendererSize().ws_col);
    printf("%c", '#');

    // Sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

std::vector<float> pongGame::getNetworkInputs() {
    return {
        ((float)ball.y / getRendererSize().ws_row) * 2.f - 1.f,
        ((float)ball.x / getRendererSize().ws_row) * 2.f - 1.f,
        ((float)aiPaddlePosition / getRendererSize().ws_row) * 2.f - 1.f
    };
}

void pongGame::setAiPaddle(float y) {
    aiPaddlePosition = (y + 1) * getRendererSize().ws_row / 2;
}
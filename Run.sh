#!/bin/bash

OUTPUT="main.out"

g++ -std=c++17 -Wall -Wextra -o $OUTPUT main.cpp Network.cpp Pong.cpp
./$OUTPUT
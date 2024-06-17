<h1 align="center">fnn</h1>

<p align="center">A general purpose, feed-forward neural network library for C++</p>

### Features
 - Fully cross-platform compilation and targeting
 - Customizable input, hidden, and output layers
 - Saving and loading trained models

### Original Use
This code was originally developed for use in a project attempting to evade heuristic fraudulent mouse movement detection. Most mouse movement APIs will instantly move the cursor to the target location, which is very easily detected and prevented. Another solution would be to gradually iterate to the target cursor location, but even this can be detected if the cursor moves in a constant direction and speed. This neural network library was used to generate movement paths (curves) that the mouse cursor would follow to appear "human" instead of moving in straight lines. The project has since concluded and I have decided to open-source this library in the hopes that it will be useful to more people. Additional information can be found [here](https://strayfade.com/FeedForward).

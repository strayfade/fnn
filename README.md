<h1 align="center">fnn</h1>
<p align="center">A general-purpose, lightweight feed-forward neural network library for C++</p>
<p align="center">
 <img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/strayfade/fnn/ubuntu-x64.yml?style=flat-square&label=Linux%20(x64)">
 <img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/strayfade/fnn/ubuntu-arm64.yml?style=flat-square&label=Linux%20(ARM64)">
 <img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/strayfade/fnn/windows-x64.yml?style=flat-square&label=Windows%20(x64)">
 <img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/strayfade/fnn/windows-arm64.yml?style=flat-square&label=Windows%20(ARM64)">
</p>

### Features
 - Cross-platform compilation and targeting (x86/ARM)
 - Customizable input, hidden, and output layer sizes
 - Serializing/deserializing trained models, saving/loading training checkpoints, etc.

### Usage
For the **Pong** demo, just compile using `make` and run using `fnn.exe` or `./fnn`. This will show an example of this FNN library playing [Pong](https://en.wikipedia.org/wiki/Pong) and using reinforcement learning to improve over time. Almost always, `main.cpp` can be modified to fit your use case, and it is a great example to build from.

> [!NOTE]
> Rendering is a bit strange depending on the platform. It renders fine on most Linux terminals, but both `cmd.exe` and Visual Studio Code's terminal fail to render the example correctly. On Windows, the Visual Studio 2022 Debugger Console renders the example correctly. On Windows, the example can also be compiled and ran using MSYS2.

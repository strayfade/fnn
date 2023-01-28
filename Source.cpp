
#ifdef _WIN32
#include <Windows.h>

#define Pen 0x00FFFFFF
HDC hdc = GetDC(GetDesktopWindow());
void DrawLine(float X1, float Y1, float X2, float Y2) {
	int a, b = 0;
	HPEN hOPen;
	HPEN hO2Pen;
	HPEN hNPen = CreatePen(PS_SOLID, (GetSystemMetrics(SM_CXSCREEN) / 960), Pen);
	HPEN hN2Pen = CreatePen(PS_SOLID, (GetSystemMetrics(SM_CXSCREEN) / 960), Pen);
	hOPen = (HPEN)SelectObject(hdc, hN2Pen);
	MoveToEx(hdc, X1, Y1, NULL);
	a = LineTo(hdc, X2, Y2);
	hO2Pen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, X1, Y1, NULL);
	b = LineTo(hdc, X2, Y2);
	DeleteObject(SelectObject(hdc, hOPen));
	DeleteObject(SelectObject(hdc, hO2Pen));
}

struct Vec2 {
	float x, y, Performance;
};
#endif

#include "Network.h"

#include <numeric>
#include <cmath>
float stddev(V(float) Input) {
	float mean = std::accumulate(Input.begin(), Input.end(), 0.0) / Input.size();
	float variance = std::inner_product(Input.begin(), Input.end(), Input.begin(), 0.0) / Input.size() - mean * mean;
	return std::sqrt(variance);
}

int main() {
	#ifdef _WIN32
	V(int) Layers = { 2, 12, 2 };

	V(NeuralNetwork) Networks;
	for (int i = 0; i < 50; i++) {
		Networks.push_back(NeuralNetwork(Layers));
	}

	while (true) {
		V(Vec2) PointsToDraw;
		V(float) PerformanceVals;
		
		// Forward on all networks
		POINT Pt;
		GetCursorPos(&Pt);
		for (int i = 0; i < Networks.size(); i++) {
			V(float) Input = { ((float)(Pt.x) / 1920) - 0.5f, ((float)(Pt.y) / 1080) - 0.5f };
			V(float) Output = Networks[i].Forward(Input);
			float Performance = pow(1 - sqrt(pow((((float)(Pt.x) - 960) / 1920) - Output[0], 2) + pow((((float)(Pt.y) - 540) / 1080) - Output[1], 2)), 5);
			Vec2 Point;
			Point.x = (Output[0] + 0.5f) * 1920;
			Point.y = (Output[1] + 0.5f) * 1080;
			Point.Performance = Performance;
			PerformanceVals.push_back(Performance);
			Networks[i].Fitness = Performance;
			PointsToDraw.push_back(Point);
			std::string t;
			for (int i = 0; i < Performance * 100; i++) {
				t += "|";
			}
			//std::cout << "Performance: " << t << std::endl;
		}
		std::cout << "stddev sigma: " << stddev(PerformanceVals) << "\n";

		// Find best performing network
		NeuralNetwork HighestFitness(Layers, false);
		for (int i = 0; i < Networks.size(); i++) {
			if (Networks[i].Fitness > HighestFitness.Fitness) {
				HighestFitness = Networks[i];
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD5)) {
			HighestFitness.Load("BestSave.nn");
		}
		if (GetAsyncKeyState(VK_NUMPAD8)) {
			HighestFitness.Save("BestSave.nn");
		}

		// Draw Points
		Vec2 T = { 0, 0, 0 };
		for (int i = 0; i < PointsToDraw.size(); i++) {
			if (PointsToDraw[i].Performance > T.Performance) {
				T = PointsToDraw[i];
			}
		}
		DrawLine(T.x, T.y, Pt.x, Pt.y);

		// Update all other networks
		for (int i = 0; i < Networks.size(); i++) {
			Networks[i].CloneFrom(HighestFitness);
			//cout << "Fitness: " << HighestFitness.Fitness << "\n";
			float Distance = sqrt(pow(Pt.x - T.x, 2) + pow(Pt.y - T.y, 2)) / 1920;
			Networks[i].Mutate(1, min(Distance, 0.1) * 0.5);
		}

		//cout << "Running!" << endl;
		Sleep(5);
	}
	#else
	_L("Tests are only supported on Windows at the moment, but Network.h still works on any operating system.", true);
	_L("To get around this, write your own tests and remove these checks!", true);
	_L("Logging can also be disabled entirely by commenting out the line where _NETWORK_LOG is defined.", true);
	#endif
}
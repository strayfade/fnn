
#ifdef _WIN32
#include <Windows.h>

#define Pen 0x00FFFFFF
HBRUSH Brush = CreateSolidBrush(Pen);
HDC hdc = GetDC(GetDesktopWindow());

void DrawPoint(float X1, float Y1)
{
	RECT rect = { X1 - 1, Y1 - 1, X1 + 1, Y1 + 1 };
	FillRect(hdc, &rect, Brush);
}

struct Vec2 {
	float x, y, Performance;
};
#endif

#include "Network.h"

int main() {
	#ifdef _WIN32
	V<int> Layers = { 2, 12, 2 };

	V<NeuralNetwork> Networks;
	for (int i = 0; i < 50; i++) {
		Networks.push_back(NeuralNetwork(Layers));
	}

	while (true) {
		V<Vec2> PointsToDraw;
		
		// Forward on all networks
		POINT Pt;
		GetCursorPos(&Pt);
		for (int i = 0; i < Networks.size(); i++) {
			//cout << Networks[i].Fitness << endl;
			V<float> Input = { ((float)(Pt.x) / 1920) - 0.5f, ((float)(Pt.y) / 1080) - 0.5f };
			auto Output = Networks[i].Forward(Input);
			float Performance = pow(1 - sqrt(pow((((float)(Pt.x) - 960) / 1920) - Output[0], 2) + pow((((float)(Pt.y) - 540) / 1080) - Output[1], 2)), 5);
			Vec2 Point;
			Point.x = (Output[0] + 0.5f) * 1920;
			Point.y = (Output[1] + 0.5f) * 1080;
			Point.Performance = Performance;
			Networks[i].Fitness = Performance;
			PointsToDraw.push_back(Point);
			string t;
			for (int i = 0; i < Performance * 100; i++) {
				t += "|";
			}
			cout << "Performance: " << t << endl;
		}

		// Find best performing network
		NeuralNetwork HighestFitness(Layers);
		for (int i = 0; i < Networks.size(); i++) {
			if (Networks[i].Fitness > HighestFitness.Fitness) {
				HighestFitness = Networks[i];
			}
		}

		// Draw Points
		Vec2 T = { 0, 0, 0 };
		for (int i = 0; i < PointsToDraw.size(); i++) {
			if (PointsToDraw[i].Performance > T.Performance) {
				T = PointsToDraw[i];
			}
		}
		DrawPoint(T.x, T.y);

		// Update all other networks
		for (int i = 0; i < Networks.size(); i++) {
			Networks[i].CloneFrom(HighestFitness);
			//cout << "Fitness: " << HighestFitness.Fitness << "\n";
			float Distance = sqrt(pow(Pt.x - T.x, 2) + pow(Pt.y - T.y, 2)) / 1920;
			Networks[i].Mutate(1 / 0.05, Distance * 0.5);
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
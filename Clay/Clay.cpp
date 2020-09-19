using namespace std;

#include <windows.h>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <chrono>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <cstdlib>

#include "Constants.h"
#include "Transformation.h"
#include "Vertex.h"
#include "Math.h"
#include "Tri.h"
#include "clayGameEngine.h"

class App : public clayGameEngine {
public:
	App() {
		m_sAppName = L"Clay";
	}

	vector<object> objects;

private:
	float fTheta;



	bool OnUserCreate() {
		object o = LoadFromObjectFile("./testCases/teapot.obj");
		/*
		Vertex v1(0.0f, 0.0f, 0.0f);
		Vertex v2(0.0f, 1.0f, 0.0f);
		Vertex v3(1.0f, 1.0f, 0.0f);
		tri t1(v1, v2, v3);

		Vertex v4(1.0f, 1.0f, 0.0f);
		Vertex v5(1.0f, 0.0f, 0.0f);
		Vertex v6(1.0f, 0.0f, 0.0f);
		tri t2(v4, v5, v6);

		Vertex v7(1.0f, 0.0f, 1.0f);
		Vertex v8(0.0f, 1.0f, 0.0f);
		Vertex v9(1.0f, 1.0f, 1.0f);
		tri t3(v7, v8, v9);

		o.tris.push_back(t1);
		o.tris.push_back(t2);
		o.tris.push_back(t3);
		*/
		objects.push_back(o);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {
		//wipe screen each time

		fTheta += 1.0f * fElapsedTime;
		Fill(0, 0, c.ScreenWidth(), c.ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		RotateZ(fTheta, objects[0]);

		return true;
	}
};


int main()
{
	App clay;
	if (clay.ConstructConsole(500, 500, 1, 1)) {
		clay.Start();
	}

}


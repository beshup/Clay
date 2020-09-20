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
#include "Vertex.h"
#include "Transformation.h"
#include "matrixMath.h"
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
		objects.push_back(o);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {
		//wipe screen each time

		fTheta += 1.0f * fElapsedTime;
		Fill(0, 0, c.ScreenWidth(), c.ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		Input(fElapsedTime);
		RotateZX(fTheta, objects[0]);

/*
    Transformation matRotZ;
    Transformation matRotX;
    Transformation transMat;

    matTrans.MakeTranslation(0.0f, 0.0f, 5.0f);
    matRotX.RotateX(fTheta);
    matRotZ.RotateZ(fTheta * 0.5f);

    Transformation matWorld;
    matWorld.MakeIdentity();
    matWorld = MultiplyMatrix(matRotZ, matRotX);
    matWorld = MultiplyMatrix(matWorld, matTrans);
*/
		return true;
	}
};


int main()
{
	App clay;
	if (clay.ConstructConsole(300, 300, 1, 1)) {
		clay.Start();
	}

}

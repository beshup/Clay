#include "clayGameEngine.h"
#include <iostream>


class Clay3D :public clayGameEngine {
	public:
		Clay3D() {
			m_sAppName = L"3D Demo";
		}

		bool OnUserCreate() override {

			DrawLine(0, 0, 100, 120);
			Vertices a = Vertices(10, 12);
			Vertices b = Vertices(2, 6);
			Vertices c = Vertices(1, 10);
			fillBottomTriangle(a, b, c);
			
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override {
			
			return true;
		}
};

int main()
{
	Clay3D clay;
	if (clay.ConstructConsole(170, 127, 6, 8)) {
		clay.Start();
	}

}


#include "clayGameEngine.h"
#include <cstdlib>
#include <iostream>

struct tri {
	Vertex vertices[3];
};

struct mesh {
	vector<tri> tris;
};

class Clay3D :public clayGameEngine {
	public:
		Clay3D() {
			m_sAppName = L"3D Demo";
		}

		bool OnUserCreate() override {

			//meshCube.tris = {

			//}
			
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override {
			Vertex a = Vertex(450, 400);
			Vertex b = Vertex(200, 300);
			Vertex c = Vertex(300, 300);
			fillBottomTriangle(a, b, c);
			return true;
		}
	private:
		mesh meshCube;
};

int main()
{
	Clay3D clay;
	if (clay.ConstructConsole(500, 500, 1, 1)) {
		clay.Start();
	}

}


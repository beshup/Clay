#include "Constants.h"
#include "Transformation.h"
#include "Vertex.h"
#include "Math.h"
#include "Tri.h"
#include "Object.h"
#include "clayGameEngine.h"
#include <cstdlib>


class Clay3D : public clayGameEngine {
	public:
		Clay3D() {
			m_sAppName = L"Clay";
		}

	private:
		float fTheta;

		bool OnUserCreate() override {

			object o;
			o.LoadFromObjectFile("hbeef.obj");
			objects.push_back(o);
		}

		bool OnUserUpdate(float fElapsedTime) override {
			//wipe screen each time

			fTheta += 1.0f * fElapsedTime;
			Fill(0, 0, c.ScreenWidth(), c.ScreenHeight(), PIXEL_SOLID, FG_BLACK);

			objects[0].RotateZ(fTheta);

			
		}
};

int main()
{
	Clay3D clay;
	if (clay.ConstructConsole(500, 500, 1, 1)) {
		clay.Start();
	}

}


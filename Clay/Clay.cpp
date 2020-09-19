#include "clayGameEngine.h"
#include <cstdlib>
#include <iostream>

struct tri {
	Vertex vertices[3];
};

struct mesh {
	vector<tri> tris;
};

struct matrix {
	float m[4][4] = { 0 };
};

class Clay3D :public clayGameEngine {
	public:
		Clay3D() {
			m_sAppName = L"Clay";
		}

	private:
		mesh meshCube;
		matrix matProj;

		float fTheta = 0;

		Vertex MultiplyMatrixVector(Vertex& p, matrix& m) {
			Vertex res;
			res.x = p.x * m.m[0][0] + p.y * m.m[0][0] + p.z * m.m[2][0] + m.m[3][0];
			res.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + m.m[3][1];
			res.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + m.m[3][2];
			// w being aritifically simulating "4th element" of input vector
			float w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + m.m[3][3];

			if (w != 0.0f) {
				res.x /= w;
				res.y /= w;
				res.z /= w;
			}

			return res;
		}

		bool OnUserCreate() override {

			meshCube.tris = {
				// SOUTH
				{ Vertex(0.0f, 0.0f, 0.0f), Vertex(0.0f, 1.0f, 0.0f), Vertex(1.0f, 1.0f, 0.0f) },
				{ Vertex(0.0f, 0.0f, 0.0f), Vertex(1.0f, 1.0f, 0.0f), Vertex(1.0f, 0.0f, 0.0f) },

				// EAST                                                      
				{ Vertex(1.0f, 0.0f, 0.0f), Vertex(1.0f, 1.0f, 0.0f), Vertex(1.0f, 1.0f, 1.0f) },
				{ Vertex(1.0f, 0.0f, 0.0f), Vertex(1.0f, 1.0f, 1.0f), Vertex(1.0f, 0.0f, 1.0f) },

				// NORTH                                                     
				{ Vertex(1.0f, 0.0f, 1.0f),    Vertex(1.0f, 1.0f, 1.0f),    Vertex(0.0f, 1.0f, 1.0f) },
				{ Vertex(1.0f, 0.0f, 1.0f),    Vertex(0.0f, 1.0f, 1.0f),    Vertex(0.0f, 0.0f, 1.0f) },

				// WEST                                                      
				{ Vertex(0.0f, 0.0f, 1.0f),   Vertex(0.0f, 1.0f, 1.0f),    Vertex(0.0f, 1.0f, 0.0f) },
				{ Vertex(0.0f, 0.0f, 1.0f),    Vertex(0.0f, 1.0f, 0.0f),    Vertex(0.0f, 0.0f, 0.0f) },

				// TOP                                                       
				{ Vertex(0.0f, 1.0f, 0.0f),    Vertex(0.0f, 1.0f, 1.0f),    Vertex(1.0f, 1.0f, 1.0f) },
				{ Vertex(0.0f, 1.0f, 0.0f),    Vertex(1.0f, 1.0f, 1.0f),    Vertex(1.0f, 1.0f, 0.0f) },

				// BOTTOM                                                    
				{ Vertex(1.0f, 0.0f, 1.0f),    Vertex(0.0f, 0.0f, 1.0f),    Vertex(0.0f, 0.0f, 0.0f) },
				{ Vertex(1.0f, 0.0f, 1.0f),    Vertex(0.0f, 0.0f, 0.0f),    Vertex(1.0f, 0.0f, 0.0f) },
			};

			//manually setting proj matrix here for now

			// Proj matrix (screen dimensions not changing for now)
			float fNear = 0.1f;
			float fFar = 1000.0f;
			float fFov = 90.0f;
			float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
			// degrees to rad
			float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.141519f);

			matProj.m[0][0] = fAspectRatio * fFovRad;
			matProj.m[1][1] = fFovRad;
			matProj.m[2][2] = fFar / (fFar - fNear);
			matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
			matProj.m[2][3] = 1.0f;
			matProj.m[3][3] = 0.0f;
			
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override {
			//wipe screen each time

			Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

			// manually setting rotZ and rotX matrices here now, remember, happens every elapsed time, so will change every fElapsedTime!

			matrix matRotZ, matRotX;

			fTheta += 1.0f * fElapsedTime;

			// Rotation Z
			matRotZ.m[0][0] = cosf(fTheta);
			matRotZ.m[0][1] = sinf(fTheta);
			matRotZ.m[1][0] = -sinf(fTheta);
			matRotZ.m[1][1] = cosf(fTheta);
			matRotZ.m[2][2] = 1;
			matRotZ.m[3][3] = 1;

			// Rotation X
			matRotX.m[0][0] = 1;
			matRotX.m[1][1] = cosf(fTheta * 0.5f);
			matRotX.m[1][2] = sinf(fTheta * 0.5f);
			matRotX.m[2][1] = -sinf(fTheta * 0.5f);
			matRotX.m[2][2] = cosf(fTheta * 0.5f);
			matRotX.m[3][3] = 1;

			for (auto triangle : meshCube.tris) {
				tri triProjected, triTranslated, triRotatedZ, triRotatedZX;

				// REFACTOR PLEASE

				//mapping each vertex to it's transformed point
				
				// Rotate in Z-Axis
				for (int i = 0; i < 3; i++) {
					triRotatedZ.vertices[i] = MultiplyMatrixVector(triangle.vertices[i], matRotZ);
				}

				// Rotate in X-Axis
				for (int i = 0; i < 3; i++) {
					triRotatedZX.vertices[i] = MultiplyMatrixVector(triRotatedZ.vertices[i], matRotX);
				}

				// Offset the object INTO the screen
				for (int i = 0; i < 3; i++) {
					triTranslated.vertices[i].z = triRotatedZX.vertices[i].z + 3.0f;
				}

				// projecting from 3D to 2D
				for (int i = 0; i < 3; i++) {
					triProjected.vertices[i] = MultiplyMatrixVector(triangle.vertices[i], matProj);
				}

				// now we gotta unnormalize (scale back up)

				// first add 1 cuz between -1 and 1, but considering pixels on console, it starts at 0 (cartesian needs to be converted to bitmap)
				// PLEASE REFACTOR THIS

				for (int i = 0; i < 3; i++) {
					triProjected.vertices[i].x += 1.0f; 
					triProjected.vertices[i].y += 1.0f;
				}

				// PLEASEEE REFCATOR THIS
				// now scale up
				for (int i = 0; i < 3; i++) {
					triProjected.vertices[i].x *= 0.5f * (float)ScreenWidth();
					triProjected.vertices[i].y *= 0.5f * (float)ScreenHeight();
				}

				//just the 3 lines after one another
				DrawTriangle(triProjected.vertices[0], triProjected.vertices[1], triProjected.vertices[2], PIXEL_SOLID, FG_WHITE);
			}
			return true;
		}
};

int main()
{
	Clay3D clay;
	if (clay.ConstructConsole(500, 500, 1, 1)) {
		clay.Start();
	}

}


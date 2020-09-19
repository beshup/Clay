#pragma once

class tri : public Constants {

	public:
		Vertex vertices[3];
		wchar_t sym = PIXEL_SOLID; //symbol for shading
		short col = FG_WHITE;   //color
		
		tri(Vertex a, Vertex b, Vertex c) {
			vertices[0] = a;
			vertices[1] = b;
			vertices[2] = c;
			sym = PIXEL_SOLID;
			col = FG_WHITE;
		}
		
		tri(const tri &t1) {
			for (int i = 0; i < 3; i++) {
				vertices[i] = t1.vertices[i];
			}
		}

		void ThreeDtoTwoD(Vertex vCamera) {
			offset();
			for (int i = 0; i < 3; i++) {
				Transformation t1("projected");
				vertices[i].MultiplyMatrixVector(t1);
			}
			// missing col?
			add1();
			unNormalization();
		}

		void RotateZ(float fTheta) {
			for (int i = 0; i < 3; i++) {
				Transformation t1;
				t1.RotateZ(fTheta);
				vertices[i].MultiplyMatrixVector(t1);
			}
		}

		void RotateX(float fTheta) {
			for (int i = 0; i < 3; i++) {
				Transformation t1;
				t1.RotateX(fTheta);
				vertices[i].MultiplyMatrixVector(t1);
			}
		}

	private:

		void unNormalization() {
			for (int i = 0; i < 3; i++) {
				vertices[i].x *= 0.5f * (float)ScreenWidth();
				vertices[i].y *= 0.5f * (float)ScreenHeight();
			}
		}

		void add1() {
			for (int i = 0; i < 3; i++) {
				vertices[i].x += 1.0f;
				vertices[i].y += 1.0f;
			}
		}

		void offset() {
			for (int i = 0; i < 3; i++)
			{
				vertices[i].z = vertices[i].z + 3.0f;
			}
		}
};

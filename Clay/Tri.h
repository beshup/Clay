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
			illumination(vCamera);
			for (int i = 0; i < 3; i++) {
				Transformation t1("projected");
				vertices[i].MultiplyMatrixVector(t1);
			}
			// missing col?
			add1();
			unNormalization();
		}

		void illumination(Vertex vCamera) {
			Vertex normal, line, line1;

			//calculate each line
			line.x = vertices[1].x - vertices[0].x;
			line.y = vertices[1].y - vertices[0].y;
			line.z = vertices[1].z - vertices[0].z;

			line1.x = vertices[2].x - vertices[0].x;
			line1.z = vertices[2].z - vertices[0].z;
			line1.y = vertices[2].y - vertices[0].y;

			Math m;
			Constants con;
			normal = m.normalize(m.crossProduct(line, line1));

			// projecting from 3D to 2D
			if (normal.x * (vertices[0].x - vCamera.x) +
				normal.y * (vertices[0].y - vCamera.y) +
				normal.z * (vertices[0].z - vCamera.z) <
				0.0)
			{

				//rudimentary illumination
				Vertex light = { 0.0f, 0.0f, -1.0f };

				m.normalize(light);

				float dotproduct = m.dotProduct(normal, light);

				CHAR_INFO c = con.GetColor(dotproduct);
				col = c.Attributes;
				sym = c.Char.UnicodeChar;
			}
		}

		void RotateZ(float fTheta) {
			for (int i = 0; i < 3; i++) {
				Transformation t1;
				t1.RotateZ(fTheta);
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

#pragma once

class object : public clayGameEngine {
	public:
        object() {}

        object(const object& o) {}

        bool LoadFromObjectFile(string filename)
        {
            ifstream f(filename);

            if (!f.is_open())
            {
                return false;
            }

            //temp storage of all vertices
            vector<Vertex> vertices;

            while (!f.eof())
            {
                char line[128]; //assume 128 characters max, if it's longer than it doesn't matter to us since we only want the coords
                f.getline(line, 128);

                strstream str;
                str << line;

                char garbage; //the info we get that we dont need

                if (line[0] == 'v')
                {
                    Vertex v;
                    str >> garbage >> v.x >> v.y >> v.z;
                    vertices.push_back(v);
                }

                if (line[0] == 'f')
                {
                    int faces[3];
                    str >> garbage;

                    for (int i = 0; i < 3; i++)
                    {
                        str >> faces[i];
                    }

                    tris.push_back({ vertices[faces[0] - 1], vertices[faces[1] - 1], vertices[faces[2] - 1] });
                }
            }

            return true;
        }

        void RotateZ(float fTheta) {
            vector<tri> toBeRasterized;
            for (auto &triangle : tris) {
                triangle.RotateZ(fTheta);
                triangle.ThreeDtoTwoD();
                toBeRasterized.push_back(triangle);
            }
            Rasterize(toBeRasterized);
        }

        void Rasterize(vector<tri> toBeRasterized) {
            sort(toBeRasterized.begin(), toBeRasterized.end(), [](tri& a, tri& b) {
                float z1Midpoint = 0.0;
                float z2Midpoint = 0.0;
                for (int i = 0; i < 3; i++)
                {
                    z1Midpoint += a.vertices[i].z;
                    z2Midpoint += b.vertices[i].z;
                }
                z1Midpoint /= 3.0f;
                z2Midpoint /= 3.0f;

                return z1Midpoint > z2Midpoint;
                });

            for (auto& triProjected : toBeRasterized)
            {

                //just the 3 lines after one another
                //FillTriangle(triProjected.vertices[0],
                //             triProjected.vertices[1],
                //             triProjected.vertices[2],
                //             triProjected.sym,
                //             triProjected.col);

                DrawTriangle(triProjected.vertices[0],
                    triProjected.vertices[1],
                    triProjected.vertices[2],
                    PIXEL_SOLID,
                    FG_DARK_RED);
            }
        }


        void DrawLine(int x0, int y0, int x1, int y1, short c = 0x2588, short col = 0x000F) {
            if (y1 - y0 > 0 && y1 - y0 < x1 - x0) {
                Bresenham(x0, y0, x1, y1, c, col);
            }
            else {
                DDA(x0, y0, x1, y1, c, col);
            }
        }

        void Bresenham(int x0, int y0, int x1, int y1, short c = 0x2588, short col = 0x000F) {
            int dx, dy, p, x, y;

            dx = x1 - x0;
            dy = y1 - y0;

            x = x0;
            y = y0;

            p = 2 * dy - dx;

            while (x < x1)
            {
                if (p >= 0)
                {
                    Draw(x, y, c, col);
                    y = y + 1;
                    p = p + 2 * dy - 2 * dx;
                }
                else
                {
                    Draw(x, y, c, col);
                    p = p + 2 * dy;
                }
                x = x + 1;
            }
        }

        void DDA(int X0, int Y0, int X1, int Y1, short c = 0x2588, short col = 0x000F)
        {
            // calculate dx & dy 
            int dx = X1 - X0;
            int dy = Y1 - Y0;

            // calculate steps required for generating pixels 
            int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

            // calculate increment in x & y for each steps 
            float Xinc = dx / (float)steps;
            float Yinc = dy / (float)steps;

            // Put pixel for each step 
            float X = X0;
            float Y = Y0;
            for (int i = 0; i <= steps; i++)
            {
                Draw(X, Y, c, col);  // put pixel at (X,Y) 
                X += Xinc;           // increment in x at each step 
                Y += Yinc;           // increment in y at each step 
           // for visualization of line- 
                                     // generation step by step 
            }
        }

        void DrawTriangle(Vertex v1, Vertex v2, Vertex v3, short c = 0x2588, short col = 0x000F)
        {
            int x1 = v1.x;
            int y1 = v1.y;
            int x2 = v2.x;
            int y2 = v2.y;
            int x3 = v3.x;
            int y3 = v3.y;
            DrawLine(x1, y1, x2, y2, c, col);
            DrawLine(x2, y2, x3, y3, c, col);
            DrawLine(x3, y3, x1, y1, c, col);
        }

        void fillBottomTriangle(Vertex v1, Vertex v2, Vertex v3) { //start at top, go ccw
          //Need inverse slope to determine the change in x for every step in y
            float slopeL = (v1.x - v2.x) / (v1.y - v2.y);
            float slopeR = (v1.x - v3.x) / (v1.y - v3.y);
            float xCurL = v2.x;
            float xCurR = v3.x;

            for (int scanY = v2.y; scanY <= v1.y; scanY++) {

                DrawLine((int)xCurL, scanY, (int)xCurR, scanY);
                xCurL += slopeL;
                xCurR += slopeR;
            }
        }

        vector<tri> tris;

};

class object : public clayGameEngine {
public:

    vector<tri> tris;

    object() {

    }
    object(string filename)
            {
                ifstream f(filename);
                Math m;

                if (!f.is_open())
                {
                    return;
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
                      //  v = m.normalize(v);
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
                        tri t(vertices[faces[0] - 1], vertices[faces[1] - 1], vertices[faces[2] - 1]);
                        tris.push_back(t);
                    }
                }

                return;
            }

            bool illumination(Vertex vCamera, tri &triangle) {
                Vertex normal, line, line1;

                //calculate each line
                Math m;
                Constants con;
                line = m.VectorSub(triangle.vertices[0], triangle.vertices[1]);
                line1 = m.VectorSub(triangle.vertices[2], triangle.vertices[1]);

                normal = m.normalize(m.crossProduct(line, line1));

                // projecting from 3D to 2D

                Vertex vCameraRay = m.VectorSub(triangle.vertices[0], vCamera);
                
                if (m.dotProduct(normal, vCameraRay) <
                    0.0f)
                { 

                  //rudimentary illumination
                  Vertex light = {0.0f, 1.0f, -1.0f};

                  light = m.normalize(light);

                  float dotproduct = max(0.1f, m.dotProduct(normal, light));

                  CHAR_INFO c = con.GetColor(dotproduct);
                  triangle.col = c.Attributes;
                  triangle.sym = c.Char.UnicodeChar;

                  count++;
                  return true;
                  
                } 
                falseCount++;
               // return true;
               return false;

            }

            void WorldToViewSpace(tri &triangle) {
                Navigation();
                MatrixMath m1;
                for (int i = 0; i < 3; i++) {
                    m1.MultiplyMatrixVector(mView, triangle.vertices[i]);
                }
            }

            void Rotate(float fTheta, bool X, bool Y, bool Z, float xSpeed = 1.0f, float ySpeed = 1.0f, float zSpeed = 1.0f) {
                vector<tri> toBeRasterized;
                vector<unique_ptr<tri>> tbrForSort;
                for (auto& triangle : tris) {
                    if (X) { triangle.RotateX(fTheta, xSpeed); }
                    if (Y) { triangle.RotateY(fTheta, ySpeed); }
                    if (Z) { triangle.RotateZ(fTheta, zSpeed); }
                    if (illumination(vCamera, triangle)) {
                        WorldToViewSpace(triangle);
                        triangle.ThreeDtoTwoD(vCamera);
                        toBeRasterized.push_back(triangle);
                    }
                    //  tbrForSort.push_back(make_unique<tri>(triangle));
                }
                Rasterize(toBeRasterized);
            }

            void translate(float x = 0.0f, float y = 0.0f, float z = 0.0f, float xSpeed = 1.0f, float ySpeed = 1.0f, float zSpeed = 1.0f ) {
                vector<tri> toBeRasterized;
                vector<unique_ptr<tri>> tbrForSort;
                for (auto& triangle : tris) {
                    triangle.translate(x, y, x, xSpeed, ySpeed, zSpeed);
                    if (illumination(vCamera, triangle)) {
                        WorldToViewSpace(triangle);
                        triangle.ThreeDtoTwoD(vCamera);
                        toBeRasterized.push_back(triangle);
                    }
                    //  tbrForSort.push_back(make_unique<tri>(triangle));
                }
                Rasterize(toBeRasterized);
            }

            void Rasterize(vector<tri> toBeRasterized) {
                sort(toBeRasterized.begin(), toBeRasterized.end(), [](tri& t1, tri& t2)
                    {
                        float z1 = (t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f;
                        float z2 = (t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f;
                        return z1 > z2;
                  });

                for (auto& triProjected : toBeRasterized)
                {

                    
                    FillTriangle(triProjected.vertices[0],
                        triProjected.vertices[1],
                        triProjected.vertices[2],
                        triProjected.sym,
                        triProjected.col
                    );
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

            void fillTopTriangle(Vertex v1, Vertex v2, Vertex v3) { //start at top, go ccw
                //Need inverse slope to determine the change in x for every step in y
                float slopeL = (v1.x - v2.x) / (v1.y - v2.y);
                float slopeR = (v1.x - v3.x) / (v1.y - v3.y);
                float xCurL = v2.x;
                float xCurR = v3.x;

                for (int scanY = v2.y; scanY > v1.y; scanY--) {
                    DrawLine((int)xCurL, scanY, (int)xCurR, scanY);
                    xCurL += slopeL;
                    xCurR += slopeR;
                }
            }

            //if(v2.y>v1.y)
            void fillBottomTriangle(Vertex v1, Vertex v2, Vertex v3) { //start at top, go ccw
                float slopeL = (v2.x - v1.x) / (v2.y - v1.y);
                float slopeR = (v2.x - v3.x) / (v2.y - v3.y);
                float xCurL = v1.x;
                float xCurR = v3.x;

                for (int scanY = v1.y; scanY < v2.y; scanY++) {
                    DrawLine((int)xCurL, scanY, (int)xCurR, scanY);
                    xCurL += slopeL;
                    xCurR += slopeR;
                }
            }

            int getTopLeft(Vertex a, Vertex b, Vertex c) {
                Vertex min = a;
                int res = 0;
                if (min.y >= b.y) {
                    if (min.y != b.y)
                        res = 1;
                    else {
                        if (min.x > b.x)
                            res = 1;
                        //error if equal : not a triangle
                    }
                }
                else if (min.y >= c.y) {
                    if (min.y != c.y)
                        res = 2;
                    else {
                        if (min.x > c.x)
                            res = 2;
                    }
                }
                return res;
            }

            int nextLeft(Vertex a, Vertex b) {
                Vertex left = a;
                int res = 0;
                if (left.x > b.x)
                    res = 1;
                return res;
            }

            int lowerY(Vertex a, Vertex b) {
                Vertex low = a;
                if (low.y > b.y)
                    low = b;
                return low.y;
            }


            void FillTriangle(Vertex v1, Vertex v2, Vertex v3, short c = 0x2588, short col = 0x000F)
            {
                int x1 = v1.x;
                int x2 = v2.x;
                int x3 = v3.x;
                int y1 = v1.y;
                int y2 = v2.y;
                int y3 = v3.y;
                auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
                auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, c, col); };

                int t1x, t2x, y, minx, maxx, t1xp, t2xp;
                bool changed1 = false;
                bool changed2 = false;
                int signx1, signx2, dx1, dy1, dx2, dy2;
                int e1, e2;
                // Sort vertices
                if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
                if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
                if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

                t1x = t2x = x1; y = y1;   // Starting points
                dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
                else signx1 = 1;
                dy1 = (int)(y2 - y1);

                dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
                else signx2 = 1;
                dy2 = (int)(y3 - y1);

                if (dy1 > dx1) {   // swap values
                    SWAP(dx1, dy1);
                    changed1 = true;
                }
                if (dy2 > dx2) {   // swap values
                    SWAP(dy2, dx2);
                    changed2 = true;
                }

                e2 = (int)(dx2 >> 1);
                // Flat top, just process the second half
                if (y1 == y2) goto next;
                e1 = (int)(dx1 >> 1);

                for (int i = 0; i < dx1;) {
                    t1xp = 0; t2xp = 0;
                    if (t1x < t2x) { minx = t1x; maxx = t2x; }
                    else { minx = t2x; maxx = t1x; }
                    // process first line until y value is about to change
                    while (i < dx1) {
                        i++;
                        e1 += dy1;
                        while (e1 >= dx1) {
                            e1 -= dx1;
                            if (changed1) t1xp = signx1;//t1x += signx1;
                            else          goto next1;
                        }
                        if (changed1) break;
                        else t1x += signx1;
                    }
                    // Move line
                next1:
                    // process second line until y value is about to change
                    while (1) {
                        e2 += dy2;
                        while (e2 >= dx2) {
                            e2 -= dx2;
                            if (changed2) t2xp = signx2;//t2x += signx2;
                            else          goto next2;
                        }
                        if (changed2)     break;
                        else              t2x += signx2;
                    }
                next2:
                    if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
                    if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
                    drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                                 // Now increase y
                    if (!changed1) t1x += signx1;
                    t1x += t1xp;
                    if (!changed2) t2x += signx2;
                    t2x += t2xp;
                    y += 1;
                    if (y == y2) break;

                }
            next:
                // Second half
                dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
                else signx1 = 1;
                dy1 = (int)(y3 - y2);
                t1x = x2;

                if (dy1 > dx1) {   // swap values
                    SWAP(dy1, dx1);
                    changed1 = true;
                }
                else changed1 = false;

                e1 = (int)(dx1 >> 1);

                for (int i = 0; i <= dx1; i++) {
                    t1xp = 0; t2xp = 0;
                    if (t1x < t2x) { minx = t1x; maxx = t2x; }
                    else { minx = t2x; maxx = t1x; }
                    // process first line until y value is about to change
                    while (i < dx1) {
                        e1 += dy1;
                        while (e1 >= dx1) {
                            e1 -= dx1;
                            if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                            else          goto next3;
                        }
                        if (changed1) break;
                        else   	   	  t1x += signx1;
                        if (i < dx1) i++;
                    }
                next3:
                    // process second line until y value is about to change
                    while (t2x != x3) {
                        e2 += dy2;
                        while (e2 >= dx2) {
                            e2 -= dx2;
                            if (changed2) t2xp = signx2;
                            else          goto next4;
                        }
                        if (changed2)     break;
                        else              t2x += signx2;
                    }
                next4:

                    if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
                    if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
                    drawline(minx, maxx, y);
                    if (!changed1) t1x += signx1;
                    t1x += t1xp;
                    if (!changed2) t2x += signx2;
                    t2x += t2xp;
                    y += 1;
                    if (y > y3) return;
                }
            }

            bool OnUserCreate() {}
            bool OnUserUpdate(float fElapsedTime){}
};
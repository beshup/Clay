#pragma once

class tri : public Constants
{

public:
  Vertex vertices[3];
  wchar_t sym = PIXEL_SOLID; //symbol for shading
  short col = FG_WHITE;      //color

  tri(Vertex a, Vertex b, Vertex c)
  {
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
    sym = PIXEL_SOLID;
    col = FG_WHITE;
  }

  tri(const tri &t1)
  {
    for (int i = 0; i < 3; i++)
    {
      vertices[i] = t1.vertices[i];
    }
  }

  void ThreeDtoTwoD(Vertex vCamera)
  {
    for (int i = 0; i < 3; i++)
    {
      Transformation t1("projected");
      MatrixMath m;
      m.MultiplyMatrixVector(t1, vertices[i]);
    }
    // missing col?
    add1();
    offset(3.0f);
    unNormalization();
  }

  void RotateZ(float fTheta)
  {
    for (int i = 0; i < 3; i++)
    {
      Transformation t1;
      t1.RotateZ(fTheta);
      MatrixMath m;
      m.MultiplyMatrixVector(t1, vertices[i]);
    }
  }

  void RotateX(float fTheta)
  {
    for (int i = 0; i < 3; i++)
    {
      Transformation t1;
      t1.RotateX(fTheta);
      MatrixMath m;
      m.MultiplyMatrixVector(t1, vertices[i]);
    }
  }

  int ClipWithPlane(Vertex plane, Vertex planeNormal, tri &toBeClipped, tri &result1, tri &result2)
  {
    Math m;
    planeNormal = m.normalize(planeNormal);

    //TEMP storage for points that fall inside view and outside
    Vertex *pointsInside[3];
    Vertex *pointsOutside[3];

    int insidePointCounter = 0;
    int outsidePointCounter = 0;

    //get distance from each point in the triangle to the plane
    float d0 = m.SignedDistance(plane, planeNormal, toBeClipped.vertices[0]);
    float d1 = m.SignedDistance(plane, planeNormal, toBeClipped.vertices[1]);
    float d2 = m.SignedDistance(plane, planeNormal, toBeClipped.vertices[2]);

    if (d0 > 0)
    {
      pointsInside[insidePointCounter] = &toBeClipped.vertices[0];
      insidePointCounter++;
    }
    else
    {
      pointsInside[outsidePointCounter] = &toBeClipped.vertices[0];
      outsidePointCounter++;
    }
    if (d1 > 0)
    {
      pointsInside[insidePointCounter] = &toBeClipped.vertices[1];
      insidePointCounter++;
    }
    else
    {
      pointsInside[outsidePointCounter] = &toBeClipped.vertices[1];
      outsidePointCounter++;
    }
    if (d2 > 0)
    {
      pointsInside[insidePointCounter] = &toBeClipped.vertices[2];
      insidePointCounter++;
    }
    else
    {
      pointsInside[outsidePointCounter] = &toBeClipped.vertices[2];
      outsidePointCounter++;
    }

    //classify triangle and break the input triangle into smaller triangles

    if (insidePointCounter == 0)
    {
      //the entire triangles out of view
      return 0;
    }
    if (insidePointCounter == 3)
    {
      //the entire triangles in view
      result1 = toBeClipped;
      return 1;
    }

    if (insidePointCounter == 1 && outsidePointCounter == 2)
    {
      //if only 1 point is in view
      //copy triangle info to resultant triangle
      result1.col = toBeClipped.col;
      result1.sym = toBeClipped.sym;
      result1.vertices[0] = *pointsInside[0];

      //get the new points
      result1.vertices[1] = m.VectorPlaneIntersection(plane, planeNormal, *pointsInside[0], *pointsOutside[0]);
      result1.vertices[2] = m.VectorPlaneIntersection(plane, planeNormal, *pointsInside[1], *pointsOutside[1]);

      return 1;
    }

    if (insidePointCounter == 2 && outsidePointCounter == 1)
    {
      //if 2 points are in view, this makes a quadrilateral which needs to be
      //divided in to 2 triangles
      //copy triangle info to resultant triangles
      result1.col = toBeClipped.col;
      result1.sym = toBeClipped.sym;
      result1.vertices[0] = *pointsInside[0];
      result1.vertices[1] = *pointsInside[1];                                                                   //first one gets both inside points
      result1.vertices[2] = m.VectorPlaneIntersection(plane, planeNormal, *pointsInside[0], *pointsOutside[0]); //third point is intersection

      result2.vertices[0] = *pointsInside[1]; //shares 2 points with result 1
      result2.vertices[1] = result1.vertices[2];
      result2.vertices[2] = m.VectorPlaneIntersection(plane, planeNormal, *pointsInside[0], *pointsOutside[0]);

      return 2;
    }
  };

private:
  void unNormalization()
  {
    for (int i = 0; i < 3; i++)
    {
      vertices[i].x *= 0.5f * (float)ScreenWidth();
      vertices[i].y *= 0.5f * (float)ScreenHeight();
    }
  }

  void add1()
  {
    for (int i = 0; i < 3; i++)
    {
      vertices[i].x += 1.0f;
      vertices[i].y += 1.0f;
    }
  }

  void offset(float offset)
  {
    for (int i = 0; i < 3; i++)
    {
      vertices[i].z = vertices[i].z + offset;
    }
  }
};

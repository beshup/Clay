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

  tri()
  {
    vertices[3];
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

  //defaulting speeds due to INTERNAL (to engine) use of the Rotates (not user-facing)
  void RotateZ(float fTheta, float speed = 1.0f)
  {
    for (int i = 0; i < 3; i++)
    {
      Transformation t1;
      t1.RotateZ(fTheta * speed);
      MatrixMath m;
      m.MultiplyMatrixVector(t1, vertices[i]);
    }
  }

  void RotateY(float fTheta, float speed = 1.0f) {
      for (int i = 0; i < 3; i++)
      {
          Transformation t1;
          t1.RotateY(fTheta * speed);
          MatrixMath m;
          m.MultiplyMatrixVector(t1, vertices[i]);
      }
  }

  void RotateX(float fTheta, float speed)
  {
    for (int i = 0; i < 3; i++)
    {
      Transformation t1;
      t1.RotateX(fTheta * speed);
      MatrixMath m;
      m.MultiplyMatrixVector(t1, vertices[i]);
    }
  }

  void translate(float x, float y, float z, float xSpeed, float ySpeed, float zSpeed) {
      for (int i = 0; i < 3; i++)
      {
          Transformation t1;
          t1.translate(x * xSpeed, y * ySpeed, z * zSpeed);
          MatrixMath m;
          m.MultiplyMatrixVector(t1, vertices[i]);
      }
  }

  int ClipWithPlane(Vertex plane, Vertex planeNormal, tri &result1, tri &result2)
  {
    Math m;
    planeNormal = m.normalize(planeNormal);

    //TEMP storage for points that fall inside view and outside
    Vertex *pointsInside[3];
    Vertex *pointsOutside[3];

    int insidePointCounter = 0;
    int outsidePointCounter = 0;

    //get distance from each point in the triangle to the plane
    float d0 = m.SignedDistance(plane, planeNormal, vertices[0]);
    float d1 = m.SignedDistance(plane, planeNormal, vertices[1]);
    float d2 = m.SignedDistance(plane, planeNormal, vertices[2]);

    if (d0 > 0)
    {
      pointsInside[insidePointCounter] = &vertices[0];
      insidePointCounter++;
    }
    else
    {
      pointsInside[outsidePointCounter] = &vertices[0];
      outsidePointCounter++;
    }
    if (d1 > 0)
    {
      pointsInside[insidePointCounter] = &vertices[1];
      insidePointCounter++;
    }
    else
    {
      pointsInside[outsidePointCounter] = &vertices[1];
      outsidePointCounter++;
    }
    if (d2 > 0)
    {
      pointsInside[insidePointCounter] = &vertices[2];
      insidePointCounter++;
    }
    else
    {
      pointsInside[outsidePointCounter] = &vertices[2];
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
      result1.vertices[0] = vertices[0];
      result1.vertices[1] = vertices[1];
      result1.vertices[2] = vertices[2];
      result1.col = col;
      result1.sym = sym;
      return 1;
    }

    if (insidePointCounter == 1 && outsidePointCounter == 2)
    {
      //if only 1 point is in view
      //copy triangle info to resultant triangle
      result1.col = col;
      result1.sym = sym;
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
      result1.col = col;
      result1.sym = sym;
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

#pragma once

class tri : public Constants
{

public:
  Vertex vertices[3];
  wchar_t sym; //symbol for shading
  short col;   //color

  tri(Vertex &a, Vertex &b, Vertex &c)
  {
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
  }

  void ThreeDtoTwoD(float fTheta, Vertex vCamera)
  {
    offset(3.0f);
    illumination(vCamera);
    for (int i = 0; i < 3; i++)
    {
      Transformation t1("world", fTheta);
      vertices[i].MultiplyMatrixVector(t1);
    }
    // missing col?
    add1();
    unNormalization();
  }

  void illumination(Vertex vCamera)
  {
    Vertex normal, line, line1;

    //calculate each line
    Math m;
    Constants con;
    line = m.VectorSub(vertices[1], vertices[0]);
    line1 = m.VectorSub(vertices[2], vertices[0]);

    normal = m.normalize(m.crossProduct(line, line1));

    // projecting from 3D to 2D

    Vertex vCameraRay = m.VectorSub(vertices[0], vCamera);
    if (m.dotProduct(normal, vCameraRay) <
        0.0)
    {

      //rudimentary illumination
      Vertex light = {0.0f, 0.0f, -1.0f};

      m.normalize(light);

      float dotproduct = m.dotProduct(normal, light);

      CHAR_INFO c = con.GetColor(dotproduct);
      col = c.Attributes;
      sym = c.Char.UnicodeChar;
    }
  }

  void RotateZ(float fTheta)
  {
    for (int i = 0; i < 3; i++)
    {
      Transformation t1;
      t1.RotateZ(fTheta);
      vertices[i].MultiplyMatrixVector(t1);
    }
  }

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

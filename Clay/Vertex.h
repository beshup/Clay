#pragma once

class Vertex
{
public:
  Vertex()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
  }
  Vertex(float x1, float y1)
  {
    x = x1;
    y = y1;
  }
  Vertex(float x1, float y1, float z1)
  {
    x = x1;
    y = y1;
    z = z1;
    w = 1.0f;
  }

  float x, y, z = 0.0f;
  float w = 1.0f;
};

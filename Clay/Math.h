#pragma once

#include "Vertex.h"

#include <cmath>

class Math
{
public:
  Math()
  {
  }

  Vertex crossProduct(Vertex vector1, Vertex vector2)
  {
    Vertex result;
    result.x = vector1.y * vector2.z - vector1.z * vector2.y;
    result.y = vector1.z * vector2.x - vector1.x * vector2.z;
    result.z = vector1.x * vector2.y - vector1.y * vector2.x;

    return result;
  }

  float dotProduct(Vertex vector1, Vertex vector2)
  {
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
  }

  float vectorMagnitude(Vertex vector)
  {
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
  }

  Vertex normalize(Vertex vector)
  {
    Vertex normal;
    float length = vectorMagnitude(vector);
    normal.x = vector.x / length;
    normal.y = vector.y / length;
    normal.z = vector.z / length;
    return normal;
  }

  Vertex VectorAdd(Vertex &a, Vertex &b)
  {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
  }
  Vertex VectorSub(Vertex &a, Vertex &b)
  {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
  }

  Vertex VectorMultiplication(Vertex &a, float k)
  {
    return {a.x * k, a.y * k, a.z * k};
  }
  Vertex VectorDivision(Vertex &a, float k)
  {
    return {a.x / k, a.y / k, a.z / k};
  }

  //Taken and modified from OneLoneCoder.com
  Transformation Invert(Transformation &mIn)
  {
    Transformation mOut;
    mOut.matrix[0][0] = mIn.matrix[0][0];
    mOut.matrix[0][1] = mIn.matrix[1][0];
    mOut.matrix[0][2] = mIn.matrix[2][0];
    mOut.matrix[0][3] = 0.0f;
    mOut.matrix[1][0] = mIn.matrix[0][1];
    mOut.matrix[1][1] = mIn.matrix[1][1];
    mOut.matrix[1][2] = mIn.matrix[2][1];
    mOut.matrix[1][3] = 0.0f;
    mOut.matrix[2][0] = mIn.matrix[0][2];
    mOut.matrix[2][1] = mIn.matrix[1][2];
    mOut.matrix[2][2] = mIn.matrix[2][2];
    mOut.matrix[2][3] = 0.0f;
    mOut.matrix[3][0] = -(mIn.matrix[3][0] * mOut.matrix[0][0] + mIn.matrix[3][1] * mOut.matrix[1][0] + mIn.matrix[3][2] * mOut.matrix[2][0]);
    mOut.matrix[3][1] = -(mIn.matrix[3][0] * mOut.matrix[0][1] + mIn.matrix[3][1] * mOut.matrix[1][1] + mIn.matrix[3][2] * mOut.matrix[2][1]);
    mOut.matrix[3][2] = -(mIn.matrix[3][0] * mOut.matrix[0][2] + mIn.matrix[3][1] * mOut.matrix[1][2] + mIn.matrix[3][2] * mOut.matrix[2][2]);
    mOut.matrix[3][3] = 1.0f;
    return mOut;
  }
};

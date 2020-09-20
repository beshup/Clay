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

  Vertex VectorPlaneIntersection(Vertex &plane, Vertex &planeNormal, Vertex &lineBegin, Vertex &lineEnd)
  {
    Vertex line = VectorSub(lineEnd, lineBegin);
    planeNormal = normalize(planeNormal);
    float planeDotProduct = dotProduct(planeNormal, plane);
    float tDenom = dotProduct(lineEnd, planeNormal) - dotProduct(lineBegin, planeNormal);
    float tNumerator = planeDotProduct - dotProduct(lineBegin, planeNormal);
    float t = tNumerator / tDenom;
    Vertex lineToIntersect = VectorMultiplication(line, t);
    return VectorAdd(lineBegin, lineToIntersect);
  }

  float SignedDistance(Vertex plane, Vertex planeNormal, Vertex point)
  {
    return (planeNormal.x * point.x + planeNormal.y * point.y + planeNormal.z * point.z - dotProduct(planeNormal, normalize(plane)));
  }
};

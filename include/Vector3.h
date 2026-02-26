#pragma once
#include <cmath>

struct Vector3 {
  float x;
  float y;
  float z;

  // Rotation of the point around the Y-axis, also known as Yaw
  Vector3 rotateY(float angle) const {
    float rad = angle;
    float newX = x * cos(rad) + z * sin(rad);
    float newZ = -x * sin(rad) + z * cos(rad);

    return {newX, y, newZ};
  }
};

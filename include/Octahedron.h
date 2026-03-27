#pragma once
#include "Object.h"
#include <cmath>

struct Octahedron : public Object {
  Vector3 center;
  float size;

  Octahedron(const Vector3& c, float s) : center(c), size(s) {}

  float sdf(const Vector3& p) const override {
    Vector3 q = {std::abs(p.x - center.x), 
                 std::abs(p.y - center.y), 
                 std::abs(p.z - center.z)};
    return (q.x + q.y + q.z - size) * 0.57735027f; // 1/sqrt(3)
  }

  int materialId() const override { return 7; }
};

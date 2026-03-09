#pragma once
#include "Object.h"
#include <algorithm>

// Tetrahedron centered at `center`, scaled by `size`
struct Tetrahedron : public Object {
  Vector3 center;
  float size;

  Tetrahedron(const Vector3& c, float s) : center(c), size(s) {}

  // SDF from IQ
  float sdf(const Vector3& p) const override {
    Vector3 q = (p - center) * (1.0f / size);
    // tetrahedron SDF approximation
    const Vector3 n1{ 1, 1, 1 };
    const Vector3 n2{ -1, -1, 1 };
    const Vector3 n3{ -1, 1, -1 };
    const Vector3 n4{ 1, -1, -1 };
    float d1 = (q.x * n1.x + q.y * n1.y + q.z * n1.z) / sqrtf(3.0f);
    float d2 = (q.x * n2.x + q.y * n2.y + q.z * n2.z) / sqrtf(3.0f);
    float d3 = (q.x * n3.x + q.y * n3.y + q.z * n3.z) / sqrtf(3.0f);
    float d4 = (q.x * n4.x + q.y * n4.y + q.z * n4.z) / sqrtf(3.0f);
    float d = std::max(std::max(d1, d2), std::max(d3, d4));
    return d * size;
  }

  int materialId() const override { return 5; }
};

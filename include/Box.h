#pragma once
#include "Object.h"
#include <cmath>
#include <algorithm>

struct Box : public Object {
  Vector3 center;
  Vector3 bounds; // half-extents x,y,z

  Box(const Vector3& c, const Vector3& b) : center(c), bounds(b) {}

  float sdf(const Vector3& p) const override {
    Vector3 q = {std::abs(p.x - center.x) - bounds.x,
                 std::abs(p.y - center.y) - bounds.y,
                 std::abs(p.z - center.z) - bounds.z};
    float max_q = std::max(q.x, std::max(q.y, q.z));
    Vector3 max_v = {std::max(q.x, 0.0f), std::max(q.y, 0.0f), std::max(q.z, 0.0f)};
    return max_v.length() + std::min(max_q, 0.0f);
  }

  int materialId() const override { return 6; }
};

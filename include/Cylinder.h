#pragma once
#include "Object.h"

// Axis-aligned capped cylinder centered at base center
struct Cylinder : public Object {
  Vector3 center; // center of cylinder
  float radius;
  float halfHeight;

  Cylinder(const Vector3& c, float r, float h) : center(c), radius(r), halfHeight(h * 0.5f) {}
  bool intersect(const Vector3& ro, const Vector3& rd, float& t, Vector3& normal) const override { return false; }

  // SDF for capped cylinder aligned along Y
  float sdf(const Vector3& p) const override {
    Vector3 q = p - center;
    // distance to infinite cylinder in XZ
    float dxz = sqrtf(q.x * q.x + q.z * q.z) - radius;
    float dy = fabs(q.y) - halfHeight;
    float ax = std::max(dxz, 0.0f);
    float ay = std::max(dy, 0.0f);
    return std::min(std::max(dxz, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
  }

  int materialId() const override { return 2; }
};




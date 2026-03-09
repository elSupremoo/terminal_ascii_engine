#pragma once
#include "Object.h"
#include <cmath>

struct Torus : public Object {
  Vector3 center;
  float majorR; // distance from center to tube center
  float minorR; // tube radius

  Torus(const Vector3& c, float R, float r) : center(c), majorR(R), minorR(r) {}

  static float sdfTorus(const Vector3& p, const Vector3& c, float R, float r) {
    Vector3 q = p - c;
    float xz = sqrtf(q.x * q.x + q.z * q.z) - R;
    return sqrtf(xz * xz + q.y * q.y) - r;
  }

  bool intersect(const Vector3& ro, const Vector3& rd, float& t, Vector3& normal) const override {
    return false;
  }

  float sdf(const Vector3& p) const override { return sdfTorus(p, center, majorR, minorR); }

  int materialId() const override { return 3; }
};

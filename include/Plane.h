#pragma once
#include "Object.h"

struct Plane : public Object {
  Vector3 point; // any point on the plane
  Vector3 normal; // should be normalized

  Plane(const Vector3& p, const Vector3& n) : point(p), normal(n) { this->normal.normalize(); }

  bool intersect(const Vector3& ro, const Vector3& rd, float& t, Vector3& outNormal) const override {
    const float EPS = 1e-6f;
    float denom = rd.dot(normal);
    if (fabs(denom) < EPS) return false; // parallel
    t = (point - ro).dot(normal) / denom;
    if (t < EPS) return false;
    outNormal = normal;
    return true;
  }
  float sdf(const Vector3& p) const override {
    // signed distance to plane along normal
    return (p - point).dot(normal);
  }

  int materialId() const override { return 4; }
};

#pragma once
#include "Object.h"
#include <cmath>

struct Sphere : public Object {
  Vector3 center;
  float radius;

  Sphere(const Vector3& c, float r) : center(c), radius(r) {}

  bool intersect(const Vector3& ro, const Vector3& rd, float& t, Vector3& normal) const override {
    return false; // not used in raymarch renderer
  }

  float sdf(const Vector3& p) const override {
    return (p - center).length() - radius;
  }

  int materialId() const override { return 1; }
};

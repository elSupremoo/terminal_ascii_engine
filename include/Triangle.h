#pragma once
#include "Object.h"

struct Triangle : public Object {
  Vector3 a, b, c;

  Triangle(const Vector3& A, const Vector3& B, const Vector3& C) : a(A), b(B), c(C) {}

  bool intersect(const Vector3& ro, const Vector3& rd, float& t, Vector3& normal) const override {
    // Moller-Trumbore
    const float EPS = 1e-6f;
    Vector3 e1 = b - a;
    Vector3 e2 = c - a;
    Vector3 p = rd.cross(e2);
    float det = e1.dot(p);
    if (fabs(det) < EPS) return false;
    float invDet = 1.0f / det;
    Vector3 s = ro - a;
    float u = s.dot(p) * invDet;
    if (u < 0.0f || u > 1.0f) return false;
    Vector3 q = s.cross(e1);
    float v = rd.dot(q) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;
    float tt = e2.dot(q) * invDet;
    if (tt > EPS) {
      t = tt;
      normal = e1.cross(e2);
      normal.normalize();
      return true;
    }
    return false;
  }
};

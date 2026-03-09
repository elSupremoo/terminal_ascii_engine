#pragma once
#include "Vector3.h"

struct Object {
  virtual ~Object() = default;

  // Traditional analytic intersection
  virtual bool intersect(const Vector3& ro, const Vector3& rd, float& t, Vector3& normal) const { return false; }

  // Signed Distance Function evaluated at point p for raymarching
  // Return distance to surface 
  virtual float sdf(const Vector3& p) const { return 1e6f; }

  // Material id 
  virtual int materialId() const { return 0; }
};

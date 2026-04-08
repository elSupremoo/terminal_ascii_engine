#pragma once
#include "Sphere.h"
#include <cmath>

struct MovingSphere : public Sphere {
  Vector3 baseCenter;
  Vector3 moveAxis;
  float speed;
  float timeOffset;
  float totalTime;

  MovingSphere(const Vector3& c, float r, const Vector3& axis, float spd, float tOffset = 0.0f) 
    : Sphere(c, r), baseCenter(c), moveAxis(axis), speed(spd), timeOffset(tOffset), totalTime(0.0f) {
      moveAxis.normalize();
  }

  void update(float dt) override {
    totalTime += dt;
    // Oscillate back and forth
    float offset = sinf(totalTime * speed + timeOffset) * 5.0f; // 5.0f amplitude
    center = baseCenter + moveAxis * offset;
  }
};

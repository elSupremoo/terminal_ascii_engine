#pragma once
#include "Vector3.h"
#include <cmath>

struct Camera {
  Vector3 pos{0.0f, 0.0f, 0.0f};
  float yaw{0.0f}; // rotation around Y
  float pitch{0.0f}; // rotation around X

  // Build a ray direction from camera through normalized screen coords 
  Vector3 getRayDirection(float nx, float ny, float fov) const {
    float aspect = 1.0f; // caller will account for aspect via nx/ny
    float tanFov = tanf(fov * 0.5f);

    // camera space
    Vector3 dir{nx * tanFov * aspect, ny * tanFov, 1.0f};

    // apply pitch (X axis) then yaw (Y axis)
    // rotate X (pitch)
    float cp = cosf(pitch), sp = sinf(pitch);
    Vector3 d1{dir.x, dir.y * cp - dir.z * sp, dir.y * sp + dir.z * cp};

    // rotate Y (yaw)
    float cy = cosf(yaw), sy = sinf(yaw);
    Vector3 d2{d1.x * cy + d1.z * sy, d1.y, -d1.x * sy + d1.z * cy};

    d2.normalize();
    return d2;
  }
};

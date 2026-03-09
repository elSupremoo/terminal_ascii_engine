#pragma once
#include <cmath>

struct Vector3 {
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};

  Vector3() = default;
  Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

  // Rotation of the point around the Y-axis (yaw)
  Vector3 rotateY(float angle) const {
    float c = cos(angle);
    float s = sin(angle);
    return {x * c + z * s, y, -x * s + z * c};
  }

  Vector3 operator-(const Vector3& o) const { return {x - o.x, y - o.y, z - o.z}; }
  Vector3 operator+(const Vector3& o) const { return {x + o.x, y + o.y, z + o.z}; }
  Vector3 operator*(float s) const { return {x * s, y * s, z * s}; }

  Vector3 cross(const Vector3& o) const {
    return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
  }

  float dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }

  float length() const { return std::sqrt(x * x + y * y + z * z); }

  void normalize() {
    float len = length();
    if (len > 1e-6f) { x /= len; y /= len; z /= len; }
  }
};

#pragma once
#include "Object.h"
#include <vector>
#include <memory>
#include <algorithm>
#include "Vector3.h"
struct Scene {
  std::vector<std::unique_ptr<Object>> objects;
  Vector3 lightDir{0.0f, -0.5f, -1.0f};

  Scene() { lightDir.normalize(); }

  void add(std::unique_ptr<Object> obj) { objects.push_back(std::move(obj)); }

  void remove(const Object* obj) {
    auto it = std::remove_if(objects.begin(), objects.end(),
                             [obj](const std::unique_ptr<Object>& o) { return o.get() == obj; });
    if (it != objects.end()) {
      objects.erase(it, objects.end());
    }
  }

  // Raymarch the scene using SDFs. Returns true if hit; sets t, normal, material, and optionally the hit object
  bool traceSDF(const Vector3& ro, const Vector3& rd, float& outT, Vector3& outNormal, int& outMat, const Object** outHitObj = nullptr) const {
    const float MAX_DIST = 200.0f;
    const int MAX_STEPS = 240;
    const float EPS = 5e-4f;

    float t = 0.0f;
    for (int i = 0; i < MAX_STEPS && t < MAX_DIST; ++i) {
      Vector3 p = ro + rd * t;
      float closest = 1e6f;
      const Object* closestObj = nullptr;
      for (const auto& o : objects) {
        float d = o->sdf(p);
        if (d < closest) { closest = d; closestObj = o.get(); }
      }
      if (closestObj == nullptr) break;
      if (closest < EPS) {
        // estimate normal via gradient
        const float h = 1e-3f;
        float dx = closestObj->sdf(p + Vector3{h,0,0}) - closestObj->sdf(p - Vector3{h,0,0});
        float dy = closestObj->sdf(p + Vector3{0,h,0}) - closestObj->sdf(p - Vector3{0,h,0});
        float dz = closestObj->sdf(p + Vector3{0,0,h}) - closestObj->sdf(p - Vector3{0,0,h});
        Vector3 n{dx, dy, dz}; n.normalize();
        outT = t; outNormal = n; outMat = closestObj->materialId();
        if (outHitObj) *outHitObj = closestObj;
        return true;
      }
      t += closest;
    }
    return false;
  }
};

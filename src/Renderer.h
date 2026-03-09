#pragma once
#include "../include/Scene.h"
#include "../include/Camera.h"
#include <string>

struct Renderer {
  int width;
  int height;
  float fovRad;
  std::string asciiChars = ".,-~:;=!*#$@";

  Renderer(int w, int h, float fovDegrees) : width(w), height(h) {
    fovRad = fovDegrees * 3.14159265f / 180.0f;
  }

  std::string render(const Scene& scene, const Camera& cam) const;
};

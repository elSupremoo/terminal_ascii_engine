#include "Renderer.h"
#include <algorithm>
#include <sstream>

// Render by casting rays per pixel and using scene.trace
std::string Renderer::render(const Scene &scene, const Camera &cam) const {
  std::string out;
  out.reserve(width * (height + 1) + 16);
  out += "\x1b[H"; // move cursor home

  float aspect = (float)width / (float)height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // normalized screen coords in [-1,1]
      float nx_base = (2.0f * (x + 0.5f) / (float)width - 1.0f);
      float ny_base = 1.0f - 2.0f * (y + 0.5f) / (float)height;
      float nx = nx_base * aspect;
      // compensate for terminal character aspect ratio
      // use a smaller charAspect to compress vertical axis and reduce tall
      // ovals
      float charAspect = 1.0f;
      float ny = ny_base * charAspect;

      Vector3 rd = cam.getRayDirection(nx, ny, fovRad);
      Vector3 ro = cam.pos;

      float t;
      Vector3 normal;
      int mat = 0;
      if (scene.traceSDF(ro, rd, t, normal, mat)) {
        // shading
        float lum = normal.dot(scene.lightDir);
        lum = std::max(0.0f, lum);
        // make materials slightly different
        float matFactor = 1.0f;
        if (mat == 2)
          matFactor = 0.8f; // cylinder
        if (mat == 3)
          matFactor = 1.2f; // torus
        if (mat == 4)
          matFactor = 0.6f; // plane
        if (mat == 5)
          matFactor = 1.0f; // tetra
        if (mat == 6)
          matFactor = 0.95f; // box
        if (mat == 7)
          matFactor = 1.1f; // octahedron
        float shadeVal = std::max(0.0f, std::min(1.0f, lum * matFactor));
        int idx = (int)(shadeVal * (asciiChars.size() - 1));
        idx = std::max(0, std::min((int)asciiChars.size() - 1, idx));

        // to render plane below horizon
        if (mat == 4) {
          // compute world hit point
          Vector3 hitp = ro + rd * t;
          // perspective spacing: use depth to scale grid
          float depth = (hitp - cam.pos).length();

          float spacing = 0.25f + depth * 0.06f;
          float gx = fmodf(hitp.x + 1000.0f, spacing);
          float gz = fmodf(hitp.z + 1000.0f, spacing);
          float lineWidth = 0.07f * spacing;
          bool isDot = (gx < lineWidth) || (gz < lineWidth);
          // fade by depth for horizon effect
          float fade = 1.0f / (0.2f * depth + 1.0f);
          if (isDot && fade > 0.12f)
            out.push_back('.');
          else
            out.push_back(' ');
        } else {
          out.push_back(asciiChars[idx]);
        }
      } else {
        out.push_back(' ');
      }

      // crosshair at the center of the screen
      if (y == height / 2 && x == width / 2) {
        out.back() = '+';
      }
    }
    if (y != height - 1)
      out.push_back('\n');
  }
  return out;
}

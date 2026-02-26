#include "../include/Vector3.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 40;
const float FOV = 20.0f;

// Bresenham algorithm to draw a line between two points
void drawLine(int x0, int y0, int x1, int y1, char *screen) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  while (true) {
    if (x0 >= 0 && x0 < SCREEN_WIDTH && y0 >= 0 && y0 < SCREEN_HEIGHT) {
      screen[y0 * SCREEN_WIDTH + x0] = '#';
    }
    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

int main() {
  // 1. Define the 5 vertices of a Pyramid
  std::vector<Vector3> pyramid = {
      {0.0f, 1.0f, 0.0f},    // 0: Apex (Top)
      {-1.0f, -1.0f, -1.0f}, // 1: Base Bottom Left
      {1.0f, -1.0f, -1.0f},  // 2: Base Bottom Right
      {1.0f, -1.0f, 1.0f},   // 3: Base Top Right
      {-1.0f, -1.0f, 1.0f}   // 4: Base Top Left
  };

  int edges[8][2] = {
      {0, 1}, {0, 2}, {0, 3}, {0, 4}, // Slopes from apex to base
      {1, 2}, {2, 3}, {3, 4}, {4, 1}  // The square base
  };

  char screen[SCREEN_WIDTH * SCREEN_HEIGHT];
  float angle = 0.0f;

  // Hard clear the terminal before the engine starts
  std::cout << "\x1b[2J";

  // game Loop
  while (true) {

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
      screen[i] = ' ';

    // 2. Process and Project each vertex
    std::vector<std::pair<int, int>> projectedPoints;
    for (Vector3 v : pyramid) {
      Vector3 rotated = v.rotateY(angle);
      rotated.z += 2.0f; // Push away from camera

      int x_proj = (SCREEN_WIDTH / 2) + (rotated.x / rotated.z) * FOV * 2;
      int y_proj = (SCREEN_HEIGHT / 2) - (rotated.y / rotated.z) * FOV;

      projectedPoints.push_back({x_proj, y_proj});
    }

    // 3. Draw the lines to the buffer
    for (auto &edge : edges) {
      int p1 = edge[0];
      int p2 = edge[1];
      drawLine(projectedPoints[p1].first, projectedPoints[p1].second,
               projectedPoints[p2].first, projectedPoints[p2].second, screen);
    }

    std::string frame = "\x1b[H"; // Reset cursor to top-left
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
      frame += screen[i];
      // line break at the end of each row
      if (i % SCREEN_WIDTH == SCREEN_WIDTH - 1) {
        frame += '\n';
      }
    }

    std::cout << frame << std::flush;

    angle += 0.05f;
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  return 0;
}

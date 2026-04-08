#include "../include/Box.h"
#include "../include/Camera.h"
#include "../include/Cylinder.h"
#include "../include/MovingSphere.h"
#include "../include/Octahedron.h"
#include "../include/Plane.h"
#include "../include/Scene.h"
#include "../include/Sphere.h"
#include "../include/Tetrahedron.h"
#include "../include/Torus.h"
#include "../include/Vector3.h"

#include "../src/Input.h"
#include "../src/Renderer.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 40;
const float FOV_DEGREES = 60.0f;

enum class GameState { MENU, PLAYING };

int main() {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
#endif

  input_init();

  Scene scenes[4];
  int currentScene = 0;
  int objectsHitCount = 0;

  float scene4Timer = 30.0f;
  int scene4Score = 0;
  bool scene4Running = true;

  GameState state = GameState::MENU;

  // Scene 0: Original
  scenes[0].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f},
                                        Vector3{0.0f, 1.0f, 0.0f}));
  scenes[0].add(std::make_unique<Sphere>(Vector3{0.5f, 0.0f, 9.0f}, 2.0f));
  scenes[0].add(
      std::make_unique<Cylinder>(Vector3{-5.0f, 0.0f, 12.0f}, 0.9f, 3.0f));
  scenes[0].add(
      std::make_unique<Torus>(Vector3{6.0f, 0.0f, 15.0f}, 3.0f, 0.6f));
  scenes[0].add(
      std::make_unique<Tetrahedron>(Vector3{0.0f, 0.8f, 15.0f}, 3.0f));
  scenes[0].add(std::make_unique<Box>(Vector3{-4.0f, -0.5f, 8.0f},
                                      Vector3{1.0f, 1.0f, 1.0f}));
  scenes[0].add(std::make_unique<Octahedron>(Vector3{4.0f, 1.0f, 10.0f}, 2.5f));

  // Scene 1: Moving Objects
  scenes[1].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f},
                                        Vector3{0.0f, 1.0f, 0.0f}));
  for (int i = 0; i < 6; ++i) {
    scenes[1].add(std::make_unique<MovingSphere>(
        Vector3{-10.0f + i * 4.0f, 1.0f, 15.0f}, 1.5f,
        Vector3{0.0f, 1.0f, 0.0f}, 2.0f, i * 1.0f));
  }

  // Scene 2:Both Static and Moving
  scenes[2].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f},
                                        Vector3{0.0f, 1.0f, 0.0f}));
  for (int i = 0; i < 4; ++i) {
    scenes[2].add(std::make_unique<Cylinder>(
        Vector3{-8.0f + i * 5.0f, 0.0f, 10.0f + (i % 2) * 2.0f}, 0.5f, 4.0f));
    scenes[2].add(std::make_unique<MovingSphere>(
        Vector3{-6.0f + i * 5.0f, 2.0f, 10.0f + ((i + 1) % 2) * 2.0f}, 1.0f,
        Vector3{1.0f, 0.0f, 0.0f}, 3.0f, i * 0.5f));
  }

  // Scene 3: Time Attack
  auto populateScene4 = [&]() {
    scenes[3].objects.clear();
    scenes[3].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f},
                                          Vector3{0.0f, 1.0f, 0.0f}));
    for (int i = 0; i < 5; ++i) {
      scenes[3].add(std::make_unique<MovingSphere>(
          Vector3{-10.0f + (rand() % 20), float(rand() % 5) + 1.0f,
                  15.0f + (rand() % 5)},
          1.0f + float(rand() % 10) / 10.0f, Vector3{1.0f, 0.0f, 0.0f},
          1.5f + float(rand() % 4), float(rand() % 10)));
    }
  };
  populateScene4();

  Camera cam;
  cam.pos = Vector3{0.0f, 0.0f, 0.0f};
  Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, FOV_DEGREES);

  std::cout << "\x1b[2J"; // clear screen

  bool running = true;
  auto last = std::chrono::high_resolution_clock::now();

  while (running) {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

    // input
    int c = input_poll();

    if (state == GameState::MENU) {
      std::cout << "\x1b[2J\x1b[H"; // Clear screen and move to top-left
      std::cout << "\x1b[1;36m";    // Cyan
      std::cout
          << "                  _______                  _             _ \n";
      std::cout
          << "                 |__   __|                (_)           | |\n";
      std::cout
          << "                    | | ___ _ __ _ __ ___  _ _ __   __ _| |\n";
      std::cout
          << "                    | |/ _ \\ '__| '_ ` _ \\| | '_ \\ / _` | |\n";
      std::cout
          << "                    | |  __/ |  | | | | | | | | | | (_| | |\n";
      std::cout
          << "                    |_|\\___|_|  |_| |_| |_|_|_| |_|\\__,_|_|\n";
      std::cout
          << "                                                           \n";
      std::cout
          << "                           _____  _____ _____ _____         \n";
      std::cout
          << "                     /\\   / ____/  ____|_   _|_   _|        \n";
      std::cout
          << "                    /  \\ | (___|  |      | |   | |          \n";
      std::cout << "                   / /\\ \\ \\___ \\  |      | |   | |     "
                   "     \n";
      std::cout
          << "                  / ____ \\____) | |____ _| |_ _| |_        \n";
      std::cout
          << "                 /_/    \\_\\_____/\\_____|_____|_____|       \n";
      std::cout
          << "                                                           \n";
      std::cout
          << "                   ______             _                    \n";
      std::cout
          << "                  |  ____|           (_)                   \n";
      std::cout
          << "                  | |__   _ __   __ _ _ _ __   ___         \n";
      std::cout
          << "                  |  __| | '_ \\ / _` | | '_ \\ / _ \\        \n";
      std::cout
          << "                  | |____| | | | (_| | | | | |  __/        \n";
      std::cout
          << "                  |______|_| |_|\\__, |_|_| |_|\\___|        \n";
      std::cout
          << "                                 __/ |                     \n";
      std::cout
          << "                                |___/                      \n\n";
      std::cout << "\x1b[1;31m                                                "
                   "by Supreme\x1b[0m\n\n\n";

      std::cout << "                              Select a Scene:\n\n";
      std::cout << "                              [1] Static Objects\n";
      std::cout << "                              [2] Moving Objects\n";
      std::cout << "                              [3] Combined Scene\n";
      std::cout << "                              [4] Time Attack Game\n\n";
      std::cout << "                              [ESC] Quit\n\n\n";
      std::cout
          << "          ==================== Controls ====================\n";
      std::cout
          << "          [W][A][S][D] Move Camera  |  [I][K][J][L] Pitch/Yaw\n";
      std::cout << "          [SPACEBAR]   Shoot Target |  [ESC]        "
                   "Return/Quit\n";
      std::cout << std::flush;

      if (c != -1) {
        if (c == 27)
          running = false;
        if (c == '1') {
          currentScene = 0;
          state = GameState::PLAYING;
        }
        if (c == '2') {
          currentScene = 1;
          state = GameState::PLAYING;
        }
        if (c == '3') {
          currentScene = 2;
          state = GameState::PLAYING;
        }
        if (c == '4') {
          currentScene = 3;
          scene4Timer = 30.0f;
          scene4Score = 0;
          scene4Running = true;
          populateScene4();
          state = GameState::PLAYING;
        }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
      continue;
    }

    if (c != -1) {
      // WASD movement
      if (c == 'w' || c == 'W')
        cam.pos = cam.pos + Vector3{0.0f, 0.0f, 0.3f}.rotateY(cam.yaw);
      if (c == 's' || c == 'S')
        cam.pos = cam.pos + Vector3{0.0f, 0.0f, -0.3f}.rotateY(cam.yaw);
      if (c == 'a' || c == 'A')
        cam.pos = cam.pos + Vector3{-0.3f, 0.0f, 0.0f}.rotateY(cam.yaw);
      if (c == 'd' || c == 'D')
        cam.pos = cam.pos + Vector3{0.3f, 0.0f, 0.0f}.rotateY(cam.yaw);
      if (c == 27) { // ESC
        state = GameState::MENU;
      }
      // arrow keys (POSIX will deliver escape sequences; basic support on
      // Windows not implemented here)
      if (c == 'j')
        cam.yaw -= 0.1f; // left
      if (c == 'l')
        cam.yaw += 0.1f; // right
      if (c == 'i')
        cam.pitch += 0.05f; // up
      if (c == 'k')
        cam.pitch -= 0.05f; // down

      // Scene switching
      if (c == '1')
        currentScene = 0;
      if (c == '2')
        currentScene = 1;
      if (c == '3')
        currentScene = 2;
      if (c == '4') {
        currentScene = 3;
        scene4Timer = 30.0f;
        scene4Score = 0;
        scene4Running = true;
        populateScene4();
      }

      // Combat / Raycasting
      if (c == ' ') {
        bool canShoot = true;
        if (currentScene == 3 && !scene4Running)
          canShoot = false;

        if (canShoot) {
          float fovRad = FOV_DEGREES * 3.14159265f / 180.0f;
          Vector3 rd = cam.getRayDirection(0.0f, 0.0f, fovRad);
          float t;
          Vector3 n;
          int mat;
          const Object *hitObj = nullptr;
          if (scenes[currentScene].traceSDF(cam.pos, rd, t, n, mat, &hitObj)) {
            if (mat != 4 && hitObj) { // ID 4 is Plane
              scenes[currentScene].remove(hitObj);
              if (currentScene == 3) {
                scene4Score++;
                // Respawn target
                scenes[3].add(std::make_unique<MovingSphere>(
                    Vector3{-10.0f + (rand() % 20), float(rand() % 5) + 1.0f,
                            15.0f + (rand() % 5)},
                    1.0f + float(rand() % 10) / 10.0f,
                    Vector3{1.0f, 0.0f, 0.0f}, 1.5f + float(rand() % 4),
                    float(rand() % 10)));
              } else {
                objectsHitCount++;
              }
            }
          }
        }
      }
    }

    if (currentScene == 3 && scene4Running) {
      scene4Timer -= dt;
      if (scene4Timer <= 0.0f) {
        scene4Timer = 0.0f;
        scene4Running = false;
      }
    }
    scenes[currentScene].update(dt);

    // render
    std::string frame = renderer.render(scenes[currentScene], cam);
    std::cout << frame;

    // draw HUD
    std::cout << "\x1b[2;2H" << "\x1b[1;36mScene: " << (currentScene + 1);
    if (currentScene == 3) {
      std::cout << "   Time Left: " << (int)scene4Timer
                << "s   Score: " << scene4Score;
      if (!scene4Running)
        std::cout << "  -- TIME'S UP! Press 4 to restart --";
    } else {
      std::cout << "   Targets Destroyed: " << objectsHitCount;
    }
    std::cout << "\x1b[0m" << std::flush;

    // frame limiter ~30fps
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }
  input_restore();
  return 0;
}

#include "../include/Vector3.h"
#include "../include/Scene.h"
#include "../include/Sphere.h"
#include "../include/Cylinder.h"
#include "../include/Torus.h"
#include "../include/Plane.h"
#include "../include/Tetrahedron.h"
#include "../include/Camera.h"
#include "../include/Box.h"
#include "../include/Octahedron.h"

#include "../src/Renderer.h"
#include "../src/Input.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 40;
const float FOV_DEGREES = 60.0f;

int main() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    input_init();

    Scene scenes[3];
    int currentScene = 0;
    int objectsHitCount = 0;

    // Scene 0: Original
    scenes[0].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f}));
    scenes[0].add(std::make_unique<Sphere>(Vector3{0.5f, 0.0f, 9.0f}, 2.0f));
    scenes[0].add(std::make_unique<Cylinder>(Vector3{-5.0f, 0.0f, 12.0f}, 0.9f, 3.0f));
    scenes[0].add(std::make_unique<Torus>(Vector3{6.0f, 0.0f, 15.0f}, 3.0f, 0.6f));
    scenes[0].add(std::make_unique<Tetrahedron>(Vector3{0.0f, 0.8f, 15.0f}, 3.0f));
    scenes[0].add(std::make_unique<Box>(Vector3{-4.0f, -0.5f, 8.0f}, Vector3{1.0f, 1.0f, 1.0f}));
    scenes[0].add(std::make_unique<Octahedron>(Vector3{4.0f, 1.0f, 10.0f}, 2.5f));

    // Scene 1: Geometric Forest
    scenes[1].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f}));
    for (int i = 0; i < 5; ++i) {
        scenes[1].add(std::make_unique<Cylinder>(Vector3{-8.0f + i * 4.0f, 0.0f, 8.0f + i * 2.0f}, 0.5f, 4.0f));
        scenes[1].add(std::make_unique<Tetrahedron>(Vector3{-6.0f + i * 4.0f, 0.0f, 10.0f + i * 2.0f}, 2.0f));
        scenes[1].add(std::make_unique<Box>(Vector3{-10.0f + i * 5.0f, -0.5f, 14.0f + i * 2.0f}, Vector3{0.8f, 0.8f, 0.8f}));
    }

    // Scene 2: Target Range
    scenes[2].add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f}));
    for (int i = 0; i < 6; ++i) {
        scenes[2].add(std::make_unique<Sphere>(Vector3{-10.0f + i * 4.0f, 1.0f, 15.0f}, 1.5f));
        scenes[2].add(std::make_unique<Torus>(Vector3{-8.0f + i * 4.0f, 4.0f, 18.0f}, 1.5f, 0.4f));
        scenes[2].add(std::make_unique<Octahedron>(Vector3{-9.0f + i * 4.0f, 7.0f, 16.5f}, 1.2f));
    }

    Camera cam;
    cam.pos = Vector3{0.0f, 0.0f, 0.0f};
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, FOV_DEGREES);

    std::cout << "\x1b[2J"; // clear screen

    bool running = true;
    auto last = std::chrono::high_resolution_clock::now();

    while (running) {
        // input
        int c = input_poll();
        if (c != -1) {
            // WASD movement
            if (c == 'w' || c == 'W') cam.pos = cam.pos + Vector3{0.0f, 0.0f, 0.3f}.rotateY(cam.yaw);
            if (c == 's' || c == 'S') cam.pos = cam.pos + Vector3{0.0f, 0.0f, -0.3f}.rotateY(cam.yaw);
            if (c == 'a' || c == 'A') cam.pos = cam.pos + Vector3{-0.3f, 0.0f, 0.0f}.rotateY(cam.yaw);
            if (c == 'd' || c == 'D') cam.pos = cam.pos + Vector3{0.3f, 0.0f, 0.0f}.rotateY(cam.yaw);
            if (c == 27) { // ESC
                running = false;
            }
            // arrow keys (POSIX will deliver escape sequences; basic support on Windows not implemented here)
            if (c == 'j') cam.yaw -= 0.1f; // left
            if (c == 'l') cam.yaw += 0.1f; // right
            if (c == 'i') cam.pitch += 0.05f; // up
            if (c == 'k') cam.pitch -= 0.05f; // down

            // Scene switching
            if (c == '1') currentScene = 0;
            if (c == '2') currentScene = 1;
            if (c == '3') currentScene = 2;

            // Combat / Raycasting
            if (c == ' ') {
                float fovRad = FOV_DEGREES * 3.14159265f / 180.0f;
                Vector3 rd = cam.getRayDirection(0.0f, 0.0f, fovRad);
                float t; Vector3 n; int mat; const Object* hitObj = nullptr;
                if (scenes[currentScene].traceSDF(cam.pos, rd, t, n, mat, &hitObj)) {
                    if (mat != 4 && hitObj) { // ID 4 is Plane
                        scenes[currentScene].remove(hitObj);
                        objectsHitCount++;
                    }
                }
            }
        }

        // render
        std::string frame = renderer.render(scenes[currentScene], cam);
        std::cout << frame;
        
        // draw HUD
        std::cout << "\x1b[2;2H" << "\x1b[1;36mScene: " << (currentScene + 1) << "   Targets Destroyed: " << objectsHitCount << "\x1b[0m";
        std::cout << std::flush;

        // frame limiter ~30fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    input_restore();
    return 0;
}

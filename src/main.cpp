#include "../include/Vector3.h"
#include "../include/Scene.h"
#include "../include/Sphere.h"
#include "../include/Cylinder.h"
#include "../include/Torus.h"
#include "../include/Plane.h"
#include "../include/Tetrahedron.h"
#include "../include/Camera.h"

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

    Scene scene;
    // Add a ground plane and a few spaced-out objects
    scene.add(std::make_unique<Plane>(Vector3{0.0f, -1.5f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f}));

    scene.add(std::make_unique<Sphere>(Vector3{0.5f, 0.0f, 9.0f}, 2.0f));
    scene.add(std::make_unique<Cylinder>(Vector3{-5.0f, 0.0f, 12.0f}, 0.9f, 3.0f));
    scene.add(std::make_unique<Torus>(Vector3{6.0f, 0.0f, 15.0f}, 3.0f, 0.6f));

    // Add a tetrahedron
    scene.add(std::make_unique<Tetrahedron>(Vector3{0.0f, 0.8f, 15.0f}, 3.0f));

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
        }

        // render
        std::string frame = renderer.render(scene, cam);
        std::cout << frame << std::flush;

        // frame limiter ~30fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    input_restore();
    return 0;
}
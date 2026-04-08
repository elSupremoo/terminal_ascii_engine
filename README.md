# Terminal ASCII Raycasting Engine
*by Supreme*

## Project Description

A compact, cross-platform ASCII-based 3D rendering engine that displays detailed 3D scenes running strictly inside a standard terminal window. By leveraging **Signed Distance Functions (SDFs)** and raymarching mathematics, this engine procedurally draws primitives like spheres, cylinders, and infinite planes entirely out of ASCII characters mapped to lighting luminance. 

The project features a **Main Menu** system and multiple playable modes, including a "Time Attack" mini-game mode where you can shoot and destroy dynamically moving 3D objects under a time limit. It serves as an experimental framework for exploring low-level computer graphics without relying on graphical frameworks like OpenGL or DirectX.

## Technologies Used

- **Language:** C++17
- **Rendering Model:** Mathematical primitive Raymarching & SDFs
- **Output:** Command Line Interface leveraging ANSI Escape Sequences for buffer clearing and text coloring.
- **Tools:** `g++` Compiler, MSYS/MinGW environments.

## Core Features

- **Geometric Primitives:** Renders Spheres, Toruses, Capped Cylinders, Tetrahedrons, Planes, Boxes, and Octahedrons simultaneously.
- **Dynamic Entities:** Features animated `MovingSphere` variants for target tracking loop systems.
- **Interactive States:** Modular `GameState` loop partitioning the stylized Main Menu sequence from the raw 3D Gameplay rendering.
- **Optimized Draw Calls:** Buffers entire frames to a single `std::string` block during the render pass to significantly mitigate latency and screen tearing.
- **Input Polling:** Simple, non-blocking polling mechanics supporting intuitive keyboard event tracking.

## Usage & Controls

### Main Menu
Select a scene by pressing the respective number on the keyboard.
- `[1]`: Load Static Objects
- `[2]`: Load Moving Objects
- `[3]`: Load Combined Scene
- `[4]`: Play Time Attack Mode
- `[ESC]`: Quit the application.

### In-Game (Playing State)
- `[W]`, `[A]`, `[S]`, `[D]`: Move Camera position forwards, backwards, and strafe sideways.
- `[I]`, `[K]`: Pitch Camera angle Up / Down
- `[J]`, `[L]`: Yaw Camera angle Left / Right
- `[Spacebar]`: Fire raycast to hit and destroy target geometries.
- `[ESC]`: Return to the Main Menu.

---

## Setup & How to Run

### Build (MSYS / MinGW on Windows)

Open an MSYS or MinGW shell and run from the `src` directory:

```bash
mkdir -p ../build
g++ -std=c++17 -O2 -I../include *.cpp -o ../build/ascii3d.exe
../build/ascii3d.exe
```

### Build (macOS / Linux)

From the `src` directory on macOS or Linux:

```bash
mkdir -p ../build
g++ -std=c++17 -O2 -I../include *.cpp -static -static-libgcc -static-libstdc++ -o ../build/engine.exe
./../build/ascii3d
```

**System Notes:**
- Ensure `g++` (GNU C++ Compiler) is installed and correctly mapped to your OS `PATH`.
- The terminal emulator running the binary natively requires support for ANSI escape codes (standardized on macOS/Linux and Windows Terminals).

---

## Project Structure

- `src/` — Contains entry initialization and hardware execution code (`main.cpp`, `Renderer.cpp`, `Input.cpp`).
- `include/` — Contains header structures, mathematical physics, and geometric objects (`Vector3.h`, `Scene.h`, `Sphere.h`, `MovingSphere.h`).
- `build/` — Output directory for compiled application binaries.

# Terminal ASCII 3D Engine

A compact, cross-platform ASCII-based 3D rendering engine that displays simple 3D scenes in a terminal using signed-distance-field (SDF) raymarching and ASCII characters for luminance.

This project is intended as an experimental, educational engine for exploring 3D rendering techniques inside plain terminal windows on Windows (MSYS/MinGW) and macOS/Linux terminals.

## Features

- Raymarched SDF primitives: sphere, torus (donut), capped cylinder, tetrahedron, and an infinite plane (ground).
- Per-pixel normal estimation and simple Lambertian shading mapped to ASCII luminance characters.
- Cross-platform non-blocking keyboard input (Windows and POSIX terminals).
- Simple camera and free movement controls.

## Build (MSYS / MinGW on Windows)

Open an MSYS or MinGW shell and run from the `src` directory:

```bash
mkdir -p ../build
g++ -std=c++17 -O2 -I../include *.cpp -o ../build/ascii3d.exe
../build/ascii3d.exe
```

## Build (macOS / Linux)

From the `src` directory on macOS or Linux:

```bash
mkdir -p ../build
g++ -std=c++17 -O2 -I../include *.cpp -o ../build/ascii3d
./../build/ascii3d
```

Notes:
- Ensure `g++` (GNU C++ compiler) is installed and available on your PATH.
- On Windows you may need to use an MSYS/MinGW environment to get a terminal that supports ANSI escape sequences.

## Usage / Controls

- `W` / `S`: move forward / backward
- `A` / `D`: strafing left / right
- `i` / `k`: pitch up / down
- `j` / `l`: yaw left / right
- `Esc`: exit

The camera movement is intentionally simple; the code is structured so you can add smooth motion, acceleration, or mouse look later.

## Project Layout

- `src/` — application entry and renderer implementation.
- `include/` — engine headers and primitive SDF implementations (`Vector3.h`, `Scene.h`, `Sphere.h`, `Torus.h`, `Cylinder.h`, `Plane.h`, `Tetrahedron.h`, etc.).
- `build/` — output folder for the compiled binary.

## Development Notes

- Rendering technique: SDF raymarching is used to get consistent silhouettes for primitives (recommended for creative ASCII scenes). The renderer estimates normals numerically and applies a simple lighting model mapped to ASCII characters.
- Terminal character aspect ratio varies by font and terminal. There is a `charAspect` constant in `src/Renderer.cpp` you can tweak to correct vertical distortion for your environment.
- Performance: raymarching with many steps per pixel is CPU-bound. Reduce `MAX_STEPS` in `include/Scene.h` for higher frame rates; increase for more accurate results.

## Suggestions / Next Steps

- Add a small configuration or runtime keys to adjust `charAspect`, field-of-view, or raymarch step budget without recompiling.
- Add additional materials, simple texturing (via patterns), and ambient occlusion approximation.
- Implement a simple scene loader or scripting to place objects at runtime.

## License

This repository is provided without an explicit license. If you intend to reuse or distribute the code, add a license file (e.g., MIT) to clarify terms.
tes

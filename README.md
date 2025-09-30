# PlanetaryObservatory

PlanetaryObservatory is a C++ OpenGL viewer for a solar system scene. The project
is actively evolving toward a component-driven scene graph with a modern
renderer, blending the original fixed-function code with new infrastructure
while the migration is underway.

## Features

- Earth and Moon rendered as textured sphere components with animation toggle
- Orbit camera supporting preset viewpoints and zooming
- Scene graph with reusable components (transform, meshes, textures, skybox, lighting)
- ImGui-powered edit mode for diagnostics, hierarchy browsing, and tooling hooks

## Build Requirements

- CMake 3.16 or newer
- C++23 compiler (Apple Clang, LLVM Clang, MSVC, or GCC)
- OpenGL, GLU, and GLFW development packages installed on the system

## Building

Configure for your preferred build type, then compile the executable:

```bash
# Debug (development default)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target PlanetaryObservatory

# Release (optimized)
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --target PlanetaryObservatory
```

For multi-config generators (e.g., Visual Studio), pass `--config <Debug|Release>`
on the `cmake --build` command instead of creating a separate build directory.

## Running

```bash
./build/PlanetaryObservatory
```

Controls: press `Tab` to toggle edit mode, `A` to pause/enable planet rotation,
and numeric keys `1`–`6` for preset camera views.

## Testing

Automated tests are not yet implemented. Once they land, run:

```bash
ctest --test-dir build
```

## Dependencies

- GLFW & legacy OpenGL (system provided)
- stb_image (vendored)
- Dear ImGui (vendored)
- GLM (via CMake FetchContent)
- Catch2 (vendored, powering future tests)

## Logging Controls

Debug logging is disabled by default. Append `-DPO_ENABLE_DEBUG_LOGS=ON` to any
`cmake -S . -B build ...` invocation to surface verbose output and runtime
diagnostics.

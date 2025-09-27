# PlanetaryObservatory

Reheated old college OpenGL project in C++

## Build Requirements

- CMake 3.16 or newer
- A modern C++23 compiler (Apple Clang, LLVM Clang, MSVC, or GCC)
- OpenGL, GLU, and GLFW development packages available on the system

## Configure & Build

```bash
# Debug configuration (default)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target PlanetaryObservatory

# Release build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target PlanetaryObservatory --config Release
```

### Visual Studio Solutions

On Windows, generate a Visual Studio solution with:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

Build and run targets directly from Visual Studio or via `cmake --build` as usual.

## Running

```bash
./build/PlanetaryObservatory
```

## Testing

Automated tests are not yet implemented. Once they land, run:

```bash
ctest --test-dir build
```

## Logging Controls

Debug logging is disabled by default. Append `-DPO_ENABLE_DEBUG_LOGS=ON` to any
`cmake -S . -B build ...` command to surface verbose output and runtime diagnostics.

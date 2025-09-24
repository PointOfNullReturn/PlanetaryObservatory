# Repository Guidelines

## Project Structure & Module Organization
PlanetaryObservatory is a single-target CMake project. Core application code lives in `src/main.cpp`; add new rendering or simulation modules under `src/`, grouping helpers into subdirectories (`src/render/`, `src/orbits/`) with matching headers. Asset files (textures, shaders) should sit under `assets/`—create the directory if absent and reference relative paths from the executable. IDE-generated directories like `cmake-build-debug/` remain untracked; keep custom build outputs inside `build/` so they stay disposable.

## Build, Test, and Development Commands
- `cmake -S . -B build` — Configure the project for your platform, regenerating build files when CMakeLists change.
- `cmake --build build --target PlanetaryObservatory` — Compile with the configured generator.
- `./build/PlanetaryObservatory` — Launch the OpenGL viewer from the build directory.
- `ctest --test-dir build` — Execute the CTest suite once tests are added; ensure it passes before submitting changes.
Use `-DCMAKE_BUILD_TYPE=Debug` during iteration and switch to `Release` when profiling or packaging.

## Coding Style & Naming Conventions
Target modern C++23 idioms. Use 4-space indentation, brace-on-same-line for functions, and `camelCase` for variables/functions while keeping `PascalCase` for types. Prefer standard library facilities over custom utilities. Run `clang-format -style=LLVM` before committing; add a local `.clang-format` override only after team discussion. Keep headers self-contained and include them via project-relative paths (`#include "render/OrbitRenderer.hpp"`).

## Testing Guidelines
Add unit tests under `tests/` and register them in CMake with `add_executable` plus `add_test`. Catch2 or GoogleTest are preferred for quick setup; document any additional dependencies. Name test files `<feature>_test.cpp` and mirror the namespace under test. Tests must mock heavy OpenGL state and focus on math/logic so they run headless. Ensure `ctest` returns zero and consider adding coverage reports via `-DCODE_COVERAGE=ON` when needed.

## Commit & Pull Request Guidelines
Follow the concise, imperative style seen in `Initial commit` (e.g., `Add orbit camera controller`). Keep subject lines under 72 characters and describe rationale in the body when non-trivial. Each PR should link related issues, outline functional changes, attach screenshots or recordings for visual updates, and note build/test results (`ctest` output). Request review once CI-equivalent checks succeed locally.

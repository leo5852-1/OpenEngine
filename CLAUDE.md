# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

OpenEngine is a minimal 3D engine/game built in C++17 on OpenGL (GLEW + GLFW + GLM), for Windows/MSVC. There is no engine editor or scene file format — game objects are constructed and wired up directly in `src/main.cpp`.

## Build

The project uses CMake with Ninja as the generator, building with MSVC (cl.exe) on Windows.

```powershell
# configure (first time / after CMakeLists.txt changes)
cmake -B build -G Ninja

# build
cmake --build build

# run (from build/, so relative shader paths resolve — see Shaders below)
.\build\OpenEngine.exe
```

There are no configured lint or test targets/frameworks in this project (no test runner, no `ctest` targets).

## Architecture

### Object model

`GameObject` (`include/gameObject.h`) is the base class for everything placed in the world: it owns a `position`, a `Collider*`, an `isStatic` flag, and an `onCollision(mtv)` virtual hook that by default just applies the minimum translation vector (MTV) to `position`. `Player`, `Cube`, and `Plane` all derive from it.

- `Player` (`include/player.h`, `src/player.cpp`) — the only dynamic (non-static) object by default. Holds camera state (`cameraOffset`/`cameraFront`/`cameraUp`), applies gravity each frame in `update(dt)`, and overrides `onCollision` to zero out vertical velocity and set grounded state when hitting floors/ceilings.
- `Cube` / `Plane` (`include/cube.h`, `include/plane.h` + `.cpp`) — static renderable primitives. Each owns its own VAO/VBO, builds interleaved position+color vertex data, and exposes `translate`/`rotate`/`scale` that mutate both the render `modelMatrix` and the physics `position`/collider size in lockstep (see `Plane::scale`, which resizes the `BoxCollider` to match visual scale).

### Collision system

`Collider` (`include/collider.h`) defines an `AABB` struct with static `checkCollision`, and a `ColliderType` enum (`Box`, `Sphere` — sphere is declared but unimplemented in `sphereCollider.h`). `BoxCollider` (`include/boxCollider.h`) computes a world-space AABB from an object's position + collider offset/size.

`CollisionSystem` (`include/collisionSystem.h`, `src/collisionSystem.cpp`) does brute-force O(n²) pairwise checks across all registered objects each `update()`:
- Skips pairs where either object has no collider, or where both are static.
- Computes an MTV via `getBoxVSBoxMTV` (least-overlap-axis resolution along whichever of x/y/z has the smallest penetration).
- Splits the MTV 50/50 between two dynamic objects, or applies it fully to whichever one is dynamic when the other is static, then calls `onCollision(mtv)` on the affected object(s).

Objects must be registered explicitly via `collisionSystem.registerObject(&obj)` (done in `main.cpp`); there is no automatic registration on construction.

### Rendering / main loop

`src/main.cpp` is the entry point and owns the GLFW window, input callbacks, and the frame loop. Per frame it: processes input → advances physics (`player.update(dt)`, `collisionSystem.update()`) → recomputes the view matrix from the player's camera state → clears the framebuffer → uploads model/view/projection uniforms → draws each object → polls/swaps.

`Shader` (`include/shader.h`, header-only) compiles/links `src/vShader.glsl` and `src/fShader.glsl` from source at startup; CMake copies both `.glsl` files into the build directory, and `main.cpp` loads them via the relative path `"../src/vShader.glsl"` — the executable is expected to be run with its working directory set to `build/`.

`Wireframe` (`include/wireframe.h`, `src/wireframe.cpp`) is a stubbed-out class with no members yet, intended for debug collider visualization.

### Third-party dependencies

`dependencies/` vendors GLFW 3.4, GLEW 2.1.0, and GLM source trees directly (added via `add_subdirectory`/`include_directories` in `CMakeLists.txt`). Treat these as read-only vendored code, not part of the engine.

Note: many comments and log messages in the codebase are written in Korean.

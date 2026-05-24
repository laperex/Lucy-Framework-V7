# Lucy Framework V7

**A real-time 3D control interface and simulation framework for a 6 Degrees-of-Freedom (6-DOF) Robotic Arm**, built with C++20, OpenGL, and an Entity Component System architecture.

---

## Overview

Lucy Framework V7 is the seventh iteration of a custom C++ application framework designed to drive, visualize, and animate a physical **6 Degrees-of-Freedom robotic arm**. The framework provides:

- A **real-time 3D viewport** powered by OpenGL and GLSL shaders for live visualization of the arm's kinematic state.
- An **immediate-mode GUI** (Dear ImGui) for controlling joint angles, running animations, and inspecting scene entities.
- An **Entity Component System (ECS)** backbone (EnTT) for clean, data-driven scene management.
- A **YAML-driven animation system** (`animator.yaml`) for scripting and replaying arm movement sequences.
- **Computer vision integration** (OpenCV) for camera-based feedback or external pose estimation.

The project targets **Linux** (the active `linux` branch) and is managed via **CMake** and **vcpkg**.

<img width="2560" height="1600" alt="Screenshot_2026-03-02-02-02-17_2560x1600" src="https://github.com/user-attachments/assets/032b9927-a4af-4b65-ace2-e1d2c6188a65" />
<img width="2560" height="1600" alt="Screenshot_2026-03-02-02-02-34_2560x1600" src="https://github.com/user-attachments/assets/6e349ea1-d748-4c3b-a971-e37382402a2b" />
<img width="2560" height="1600" alt="Screenshot_2026-03-02-03-17-38_2560x1600" src="https://github.com/user-attachments/assets/eac0f9eb-24a5-4b61-b37f-7d4efb9268e9" />

---

## Features

| Feature | Details |
|---|---|
| 3D Arm Visualization | OpenGL + GLAD rendering of loaded 3D arm models |
| GLSL Shaders | Custom vertex/fragment shaders for realistic rendering |
| Interactive GUI | Dear ImGui panels for joint control and scene inspection |
| 3D Gizmos | ImGuizmo for drag-to-rotate/translate in the 3D viewport |
| Entity Component System | EnTT-powered scene graph — entities, transforms, meshes |
| Model Loading | Assimp for importing 3D arm mesh assets |
| Animation Playback | YAML-configured animation sequences via yaml-cpp |
| Computer Vision | OpenCV integration for camera or vision-based arm feedback |
| Math Library | GLM for vectors, matrices, and quaternion kinematics |
| UUID Support | stduuid for unique entity identification |
| Image Loading | STB for texture and asset loading |
| Input & Windowing | SDL2 for cross-platform window management and input |

---

## Architecture

The framework is organized around a **data-oriented Entity Component System** using EnTT. The high-level layers are:

```
┌──────────────────────────────────────────────────┐
│               Application Entry Point            │
├──────────────────────────────────────────────────┤
│         Lucy Core Framework (ECS + Systems)      │
│  ┌────────────┐  ┌────────────┐  ┌─────────────┐ │
│  │   Scene    │  │  Renderer  │  │  Animator   │ │
│  │ (EnTT ECS) │  │ (OpenGL)   │  │ (YAML)      │ │
│  └────────────┘  └────────────┘  └─────────────┘ │
├──────────────────────────────────────────────────┤
│              Editor / GUI Layer                  │
│         Dear ImGui  +  ImGuizmo Gizmos           │
├──────────────────────────────────────────────────┤
│              Vision / Input Layer                │
│          OpenCV Camera  +  SDL2 Events           │
├──────────────────────────────────────────────────┤
│        Platform: Linux  |  OpenGL / GLSL         │
└──────────────────────────────────────────────────┘
```

### Key Subsystems

**Scene & ECS** — Built on `EnTT`, every joint, link, camera, and light in the arm scene is an entity. Components store transform data (GLM matrices/quaternions), mesh handles, and material properties.

**Renderer** — GLAD loads OpenGL extensions at runtime. GLSL shader programs handle vertex transformation and fragment shading. Assimp imports `.obj`/`.fbx`/`.dae` arm meshes into GPU buffers.

**Animator** — The `animator.yaml` file declares named animation sequences (joint angle keyframes). The animation system ticks through keyframes each frame and writes joint angles back to ECS transform components.

**GUI & Gizmos** — Dear ImGui renders docked panels for joint sliders, entity inspector, and animation playback controls. ImGuizmo overlays 3D transform handles directly in the viewport.

**Vision** — OpenCV can read from a connected camera to provide real-world arm pose feedback or ArUco marker–based tracking.

---

## Dependencies

All dependencies are managed via **vcpkg**. The following packages are required:

| Library | Purpose |
|---|---|
| `sdl2` | Windowing, OpenGL context creation, input events |
| `imgui[sdl2-binding,opengl3-binding]` | Immediate-mode GUI |
| `imguizmo` | 3D transform gizmos in the viewport |
| `assimp` | 3D model import (arm meshes) |
| `yaml-cpp` | YAML animation config parsing |
| `stb` | Image/texture loading |
| `entt` | Entity Component System |
| `glm` | Mathematics (vectors, matrices, quaternions) |
| `glad` | OpenGL extension loader |
| `stduuid` | UUID generation for entities |
| `opencv4` | Computer vision / camera input |

---

## Prerequisites

- **OS**: Linux (Ubuntu 20.04+ recommended)
- **Compiler**: GCC 10+ or Clang 12+ with C++20 support
- **CMake**: 3.0 or later
- **vcpkg**: Installed and bootstrapped at `~/.local/share/vcpkg` (or update the path in `CMakeLists.txt`)
- **OpenGL**: Mesa or proprietary GPU drivers with OpenGL 3.3+ support

### Install vcpkg

```bash
git clone https://github.com/microsoft/vcpkg.git ~/.local/share/vcpkg
cd ~/.local/share/vcpkg
./bootstrap-vcpkg.sh
```

### Install dependencies via vcpkg

```bash
vcpkg install sdl2 imgui[sdl2-binding,opengl3-binding] imguizmo assimp yaml-cpp stb entt glm glad stduuid opencv4
```

---

## Building

```bash
# 1. Clone the repository
git clone https://github.com/laperex/Lucy-Framework-V7.git
cd Lucy-Framework-V7

# 2. Checkout the linux branch (default active branch)
git checkout linux

# 3. Configure with CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 4. Build
cmake --build build --parallel

# 5. Run
./build/main
```

> **Note**: The `CMakeLists.txt` hardcodes vcpkg paths to `/home/laperex/.local/share/vcpkg`. Update the `include()` path and `include_directories()` to match your own vcpkg installation:
>
> ```cmake
> include(/YOUR/PATH/vcpkg/scripts/buildsystems/vcpkg.cmake)
> include_directories("./dependencies/include/" include src /YOUR/PATH/vcpkg/installed/x64-linux/include)
> ```

---

## Project Structure

```
Lucy-Framework-V7/
├── src/                    # C++ source files (framework core + application)
├── assets/                 # 3D models, textures, GLSL shaders
├── CMakeLists.txt          # CMake build configuration
├── animator.yaml           # YAML animation sequence definitions
├── .editorconfig           # Editor formatting rules
├── .gitignore
└── LICENSE                 # MIT License
```

### Source Layout (inferred)

```
src/
├── Lucy/                   # Core framework (ECS, renderer, window, input)
│   ├── Core/               # Application loop, events, layer stack
│   ├── ECS/                # EnTT wrappers, components, systems
│   ├── Renderer/           # OpenGL abstraction (shaders, VAO, FBO, textures)
│   └── Editor/             # ImGui panels, viewport, gizmos
├── RoboticArm/             # Arm-specific logic (kinematics, joint control)
└── main.cpp                # Entry point
```

> The exact source tree may vary. The above is inferred from the CMake glob pattern `src/**/*.cpp`.

---

## Configuration

### `animator.yaml`

This file defines animation sequences for the robotic arm. Each entry represents a named animation with a list of keyframes specifying joint angles over time. Example schema:

```yaml
animations:
  - name: "pick_and_place"
    keyframes:
      - time: 0.0
        joints: [0, 0, 0, 0, 0, 0]
      - time: 1.5
        joints: [45, -30, 60, 0, 90, 0]
      - time: 3.0
        joints: [0, 0, 0, 0, 0, 0]
```

The animation system reads this file at startup and exposes playback controls through the ImGui interface.

---

## License

This project is licensed under the **MIT License**. See [LICENSE](LICENSE) for details.

---

*Lucy Framework V7 — 6 DOF Robotic Arm Control Interface*

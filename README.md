# Curloz Engine

A Vulkan 1.3 game engine written in C++17. 

> This README is for contributors and maintainers. If you're just browsing, check out [VKLearn](https://vklearn.org) for the Vulkan tutorials that came out of building this.

---

## Tech Stack

| System | Library |
|---|---|
| Renderer | Vulkan 1.3 (dynamic rendering) |
| Windowing | GLFW |
| ECS | EnTT* |
| Physics | Jolt Physics* |
| Audio | OpenAL Soft* |
| Math | GLM* |
| Model Loading | fastgltf* |
| Animation | ozz-animation* |
| Build System | CMake + Ninja |

---
> '*' Signifies that system is planned and has not yet been implemented yet
## Prerequisites

### Linux
- Vulkan 1.3 capable GPU and drivers
- CMake 3.25+
- Ninja
- clang-format (for code style enforcement)
- GCC or Clang with C++17 support

On Gentoo:
```bash
sudo emerge -av cmake ninja clang dev-util/vulkan-tools
```

On Ubuntu/Debian:
```bash
sudo apt install cmake ninja-build clang-format vulkan-tools libvulkan-dev
```

### Windows
- CMake 3.25+
- Ninja
- Vulkan SDK from [lunarg.com](https://vulkan.lunarg.com)
- MSVC or MinGW with C++17 support

---

## Building

```bash
# Clone with submodules
git clone --recursive https://github.com/curl0z/curloz-engine.git
cd curloz-engine

# If you forgot --recursive
git submodule update --init --recursive

# Configure and build
cmake -B build -G Ninja
cmake --build build
```

Binary lands at `build/CurlozEngine`.

---

## Project Structure

```
curloz-engine/
├── src/                # Source files (.cpp)
├── include/            # Header files (.hpp)
├── shaders/            # GLSL shaders, compiled to .spv
├── assets/             # Models, textures, audio
├── external/           # Git submodules (do not modify manually)
│   ├── glfw/
│   ├── entt/
│   └── jolt/
├── .clang-format
├── .editorconfig
├── CMakeLists.txt
└── LICENSE
```

---


## License

MIT
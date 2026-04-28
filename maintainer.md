# Maintainer Guide

This document is for contributors working on Curloz Engine. Read this before writing a single line.

---

## Who Owns What

| Area | Owner      | Scope |
|---|------------|---|
| Renderer, Vulkan, pipelines, sync, swapchain | @curl0z    | Everything under `src/renderer/` |
| ECS, EnTT systems, components | unassigned | Everything under `src/ecs/` |
| Physics, Jolt integration | unassigned | Everything under `src/physics/` |

**Do not touch code outside your area without discussing it first.** If you need something from another area, ask. Don't patch it yourself.


---

## Dev Environment Setup

### Linux

```bash
# Clone with submodules
git clone --recursive https://github.com/curl0z/curloz-engine.git
cd curloz-engine

# Install deps (Gentoo)
sudo emerge -av cmake ninja clang vulkan-tools

# Install deps (Ubuntu/Debian)
sudo apt install cmake ninja-build clang-format libvulkan-dev vulkan-tools

# Build
cmake -B build -G Ninja
cmake --build build
```

### Windows

- Install [Git for Windows](https://git-scm.com/download/win) (includes Git Bash — required)
- Install [CMake](https://cmake.org/download/)
- Install [Ninja](https://ninja-build.org/)
- Install [Vulkan SDK](https://vulkan.lunarg.com/)
- Open the repo folder in Visual Studio via **File → Open → Folder** (not Open Project)
- VS detects `CMakeLists.txt` automatically — no `.vcxproj` files

---

## Git Workflow

### Branches

```
main                  → always stable, always builds
feature/xxx           → new features
fix/xxx               → bug fixes
```

Never commit directly to `main`. Always branch, then PR.

```bash
# Start work
git checkout -b feature/physics-rigidbody

# Push branch
git push --set-upstream origin feature/physics-rigidbody

# Delete after merge
git branch -d feature/physics-rigidbody
git push origin --delete feature/physics-rigidbody
```

### Commit Messages

Format: `type: short description`

```
feature: added xyz feature
fix: fixed xyz bug
refactor: cleaned the messy code at xyz system/file
```

Types: `feature`, `fix`, `refactor`

No "fixed stuff". No "wip" on main. No giant commits that touch everything.

### PRs

- Every branch merge goes through a PR, even between friends
- Write what you did and why in the PR description
- At least one review before merge

---

## Code Style

- **Indentation** — tabs, 8-wide
- **Braces** — Allman everywhere (brace always on its own line)
- **Pointers** — left aligned (`int* ptr`)
- **Classes** — `PascalCase`
- **Members/variables** — `camelCase`
- **Statics** — `s_` prefix

Enforced by `.clang-format` and `.editorconfig` in the repo root.

### Setup (Linux)

```bash
# Install clang-format
sudo emerge -av clang  # Gentoo
sudo apt install clang-format  # Ubuntu

# Set up pre-commit hook (one time)
cp scripts/pre-commit .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

### Setup (Windows)

clang-format is built into Visual Studio. Enable it:

Tools → Options → Text Editor → C/C++ → Code Style → Formatting → **Enable ClangFormat support** → check **Format on Save**

VS picks up `.clang-format` from the repo root automatically.

### Fix Formatting Manually

```bash
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

---

## Doxygen Comment Standards

Every file, struct, and function must be documented. Use `///` style comments.

### File Header

Every `.cpp` and `.hpp` file starts with:

```cpp
/**
 * @file PhysicsSystem.hpp
 * @author Your Name
 * @brief Brief one line description of what this file contains
 */
```

### Structs and Classes

```cpp
/**
 * @brief Stores position, rotation and scale of an entity in world space.
 *
 * This is the handshake between the ECS and physics systems.
 * Do not modify the layout without consulting @curl0z.
 */
struct TransformComponent
{
        glm::vec3 position; ///< World space position
        glm::quat rotation; ///< Rotation as quaternion
        glm::vec3 scale;    ///< Non-uniform scale
};
```

### Functions

```cpp
/**
 * @brief Submits a mesh for rendering this frame.
 *
 * @param data Mesh geometry and material data to submit
 * @return True if submission succeeded, false if render queue is full
 */
bool submitMesh(const MeshSubmitData& data);
```

### Enum

```cpp
/**
 * @brief Describes the current state of the physics body.
 */
enum class BodyState
{
        Active,   ///< Body is being simulated
        Sleeping, ///< Body has come to rest
        Static    ///< Body never moves
};
```

### Quick Reference

| Tag | Use |
|---|---|
| `@file` | File name |
| `@author` | Who wrote it |
| `@brief` | One line summary |
| `@param` | Function parameter |
| `@return` | Return value |
| `@note` | Extra info |
| `@warning` | Something that can go wrong |
| `@todo` | Planned work |
 
---
## Doxygen Docs

Docs are **not committed to the repo**. They are auto-generated and hosted via GitHub Pages on every push to `main`.

To generate locally:

```bash
doxygen Doxyfile
```

Output goes to `docs/` which is in `.gitignore`. Do not commit it.

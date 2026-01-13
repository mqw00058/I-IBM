# I-IBM
**GUI-based 3D Surface Processing Tool**

<video src="demo.mp4"
       autoplay
       muted
       loop
       playsinline
       width="720"></video>


*Demo performing 3D surface visualization/processing in I-IBM interface.*

---

## Overview
**I-IBM** is a **GUI-based application** for **3D surface processing and visualization**. Through a Qt widget UI, you can intuitively control rendering elements such as **vertex/edge/face toggles**, **vertex normals**, **Bounding Box / Workspace / Axis**. It is suitable for computational geometry, mesh manipulation, and visualization workflows.

---

## Features
- **3D Surface Visualization**: Toggle display of Vertex/Edge/Face/Normals
- **Selection Modes**: Points / Faces selection mode
- **Scene Context Controls**: Toggle Bounding Box, Workspace, Axis
- **Log View**: Check status/file operation messages
- **Extensible Structure**: Ready for plugin-based feature additions

---

## System Requirements
- **CPU**: x86_64  
- **RAM**: ≥ 4 GB (8 GB recommended for large mesh processing)  
- **GPU**: OpenGL 3.3+ driver (recommended for smooth rendering)  
- **Disk**: ≥ 200 MB free space  
- **Toolchain**: C++11 or higher (MSVC / Clang / GCC)

### Software Dependencies
- **Qt** 5.x or 6.x (Widgets, Core, Gui)
- **OpenMesh**
- **OpenCV**
- **Numerical Recipes** (License compliance required when using source/header)
- **CMake ≥ 3.16** or **qmake** (either can be used for build)

> **Windows Note:** Ensure pre-built libraries (e.g., Qt/OpenMesh/OpenCV) match the **MSVC toolset**. For example, if Qt uses `msvc2015_64 (v140)`, then project and linked libraries must also be built with **v140 + x64** to avoid link/runtime issues.

---

## Supported Platforms
- **Windows 10/11 (x64)** — Qt 5.x/6.x + MSVC v140/v141/v142/v143
- **Linux (x86_64)** — Qt 5.x/6.x + GCC/Clang, OpenGL driver required

> macOS is not officially tested yet. Contributions for CI/platform adjustments are welcome.

---

## Libraries Used
- OpenCV — Image processing and utilities  
- OpenMesh — Mesh data structures/algorithms  
- Numerical Recipes — Numerical analysis techniques (check license)  
- Qt — Cross-platform GUI framework

---

## Getting Started

### Clone
```bash
git clone https://github.com/<your-username>/I-IBM.git
cd I-IBM
```

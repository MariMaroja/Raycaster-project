# Raycaster Project

A simple raycasting engine built with C++ and SFML, inspired by early first-person games such as Wolfenstein 3D.

This project was created to practice graphics programming, game engine fundamentals, trigonometry, and real-time rendering concepts.

## Features

* 2D minimap visualization
* First-person pseudo-3D rendering using raycasting
* Player movement with collision detection
* Directional camera system
* Mouse-based camera rotation
* Distance-based wall shading
* Floor and ceiling rendering

## Technologies Used

* C++
* SFML
* CMake

## Project Structure

```txt
raycaster-project/
│── src/
│   └── main.cpp
│── build/
│── CMakeLists.txt
│── README.md
```

## How It Works

The engine uses a raycasting technique to simulate a 3D environment from a 2D map.

Each frame:

1. Rays are cast from the player's position.
2. The engine checks wall intersections.
3. Distance is calculated to determine wall height.
4. Vertical slices are rendered to create a pseudo-3D scene.
5. Brightness is adjusted based on distance to simulate depth.

## Controls

| Key   | Action        |
| ----- | ------------- |
| W     | Move Forward  |
| S     | Move Backward |
| A     | Move Left     |
| D     | Move Right    |
| Mouse | Rotate Camera |

## Build Instructions

### Requirements

* C++17 or higher
* SFML
* CMake

### Clone the repository

```bash
git clone <your-repository-url>
cd raycaster-project
```

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

```bash
./RaycasterProject
```

On Windows:

```bash
RaycasterProject.exe
```

## Learning Goals

This project was developed to improve knowledge in:

* Game programming fundamentals
* Rendering techniques
* Raycasting logic
* Collision systems
* Trigonometry in games
* C++ project organization

## Future Improvements

* Wall textures
* Enemy entities
* Interactive objects
* Better lighting system
* Level editor
* Performance optimizations

## Preview

Add screenshots or a GIF here after finishing the project.

---

Made for learning and portfolio purposes.

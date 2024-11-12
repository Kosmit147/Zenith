<div align="center">

# Zenith

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/Kosmit147/Zenith/blob/master/LICENSE.md)
[![Build](https://github.com/Kosmit147/Zenith/actions/workflows/build.yml/badge.svg)](https://github.com/Kosmit147/Zenith/actions/workflows/build.yml)

A 3D video game engine made for a university project.

</div>

## How to Use

1. Make sure to clone this repository recursively with the ```--recursive``` option in git to clone the project's dependencies as well.
	```
	git clone --recursive https://github.com/Kosmit147/Zenith.git Zenith
	```
1. If you use Linux, install GLFW's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:
    ```
    sudo apt update
	sudo apt install \
        extra-cmake-modules \
        libwayland-dev \
        libxcursor-dev \
        libxi-dev \
        libxinerama-dev \
        libxkbcommon-dev \
        libxrandr-dev \
        wayland-protocols
    ```
1. Configure and build your project using CMake. Most popular IDEs support CMake projects with very little effort on your part.
    - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
    - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
    - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
    - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

    Alternatively, run these commands using the terminal. Be sure to run them in the root directory of the project you just created.
    ```
    cmake -B build
    cmake --build build
    ```

## License

Zenith is licensed under the MIT License.

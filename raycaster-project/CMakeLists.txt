cmake_minimum_required(VERSION 3.16)

project(RaycasterProject)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(RaycasterProject
    src/main.cpp
)

target_link_libraries(RaycasterProject
    SFML::Graphics
    SFML::Window
    SFML::System
)
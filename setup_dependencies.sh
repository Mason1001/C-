#!/bin/bash

# This script downloads and sets up dependencies for the Mouse Tracker application

echo "Setting up dependencies for Mouse Tracker..."

# Create libs directory
mkdir -p libs
cd libs

# Download ImGui
echo "Downloading ImGui..."
if [ ! -d "imgui" ]; then
    git clone --depth 1 --branch v1.89.9 https://github.com/ocornut/imgui.git
    echo "ImGui downloaded successfully"
else
    echo "ImGui already exists, skipping..."
fi

# Download GLFW
echo "Downloading GLFW..."
if [ ! -d "glfw" ]; then
    git clone --depth 1 --branch 3.3.8 https://github.com/glfw/glfw.git
    echo "GLFW downloaded successfully"
else
    echo "GLFW already exists, skipping..."
fi

# Download nlohmann/json
echo "Downloading nlohmann/json..."
if [ ! -d "nlohmann" ]; then
    mkdir -p nlohmann
    curl -o nlohmann/json.hpp https://raw.githubusercontent.com/nlohmann/json/v3.11.2/single_include/nlohmann/json.hpp
    echo "nlohmann/json downloaded successfully"
else
    echo "nlohmann/json already exists, skipping..."
fi

cd ..

echo "All dependencies downloaded successfully!"
echo "You can now build the project using:"
echo "  mkdir build && cd build"
echo "  cmake .."
echo "  make"

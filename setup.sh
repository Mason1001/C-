#!/bin/bash

# Setup script for Mouse Tracker Application
# This script downloads and sets up all dependencies

set -e

echo "=== Mouse Tracker - Dependency Setup ==="
echo ""

# Create external directory if it doesn't exist
mkdir -p external
cd external

# Clone ImGui
if [ ! -d "imgui" ]; then
    echo "Downloading ImGui..."
    git clone --depth 1 --branch docking https://github.com/ocornut/imgui.git
    echo "ImGui downloaded successfully"
else
    echo "ImGui already exists"
fi

# Clone nlohmann/json
if [ ! -d "json" ]; then
    echo "Downloading nlohmann/json..."
    git clone --depth 1 --branch v3.11.2 https://github.com/nlohmann/json.git
    echo "nlohmann/json downloaded successfully"
else
    echo "nlohmann/json already exists"
fi

cd ..

echo ""
echo "=== Dependencies setup complete ==="
echo ""
echo "Next steps:"
echo "1. Install system dependencies:"
echo "   Ubuntu/Debian: sudo apt-get install libsdl2-dev libgl1-mesa-dev"
echo "   Fedora: sudo dnf install SDL2-devel mesa-libGL-devel"
echo "   Arch: sudo pacman -S sdl2 mesa"
echo ""
echo "2. Build the project:"
echo "   ./build.sh"
echo ""

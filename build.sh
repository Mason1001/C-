#!/bin/bash

# Build script for Mouse Tracker Application

set -e

echo "=== Building Mouse Tracker ==="
echo ""

# Create build directory
mkdir -p build
cd build

# Run CMake
echo "Running CMake configuration..."
cmake ..

# Build
echo "Building project..."
cmake --build . -j$(nproc)

echo ""
echo "=== Build complete ==="
echo ""
echo "Executable: build/MouseTracker"
echo "Run with: ./build/MouseTracker"
echo ""

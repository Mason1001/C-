# Building Mouse Tracker

This guide provides detailed instructions for building the Mouse Tracker application from source.

## Prerequisites

### Linux (Ubuntu/Debian)

```bash
# Update package list
sudo apt-get update

# Install build essentials
sudo apt-get install -y build-essential cmake git

# Install OpenGL and X11 development libraries
sudo apt-get install -y libgl1-mesa-dev libx11-dev libxrandr-dev
sudo apt-get install -y libxinerama-dev libxcursor-dev libxi-dev
sudo apt-get install -y libxext-dev libxtst-dev

# Install GLEW for OpenGL extension loading
sudo apt-get install -y libglew-dev
```

### Windows

1. **Install Visual Studio**
   - Download and install Visual Studio 2019 or later
   - During installation, select "Desktop development with C++"
   - Ensure CMake tools are included

2. **Install CMake**
   - Download from [cmake.org](https://cmake.org/download/)
   - Add CMake to system PATH during installation

3. **Install Git**
   - Download from [git-scm.com](https://git-scm.com/)
   - Use default installation options

### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake git glew
```

## Quick Build

### One-Command Build (Linux/macOS)

```bash
# Clone, setup dependencies, and build
git clone https://github.com/Mason1001/C-.git && \
cd C- && \
./setup_dependencies.sh && \
mkdir build && cd build && \
cmake .. && make -j$(nproc)
```

### Step-by-Step Build

#### 1. Clone the Repository

```bash
git clone https://github.com/Mason1001/C-.git
cd C-
```

#### 2. Download Dependencies

The project uses several third-party libraries that need to be downloaded:

```bash
# On Linux/macOS
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# On Windows (using Git Bash or WSL)
bash setup_dependencies.sh
```

This script will download:
- **Dear ImGui** v1.89.9 - Immediate mode GUI library
- **GLFW** 3.3.8 - Window and input handling
- **nlohmann/json** v3.11.2 - JSON parsing library

#### 3. Configure the Build

```bash
mkdir build
cd build
cmake ..
```

**CMake Configuration Options:**

- `-DCMAKE_BUILD_TYPE=Release` - Build with optimizations (default is Debug)
- `-DGLFW_BUILD_EXAMPLES=OFF` - Don't build GLFW examples (saves time)
- `-DGLFW_BUILD_TESTS=OFF` - Don't build GLFW tests

Example:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF
```

#### 4. Build the Project

**Linux/macOS:**
```bash
make -j$(nproc)  # Uses all CPU cores
```

**Windows (Visual Studio):**
```bash
cmake --build . --config Release
```

**Windows (MinGW):**
```bash
mingw32-make -j%NUMBER_OF_PROCESSORS%
```

#### 5. Run the Application

**Linux/macOS:**
```bash
./MouseTracker
```

**Windows:**
```bash
Release\MouseTracker.exe
```

## Build Output

After a successful build, you should see:

```
[100%] Built target MouseTracker
```

The executable will be located in:
- **Linux/macOS**: `build/MouseTracker`
- **Windows**: `build/Release/MouseTracker.exe` or `build/Debug/MouseTracker.exe`

Expected binary size: ~2-3 MB

## Troubleshooting

### Common Issues

#### CMake Can't Find OpenGL

**Linux:**
```bash
sudo apt-get install libgl1-mesa-dev
```

**macOS:**
```bash
brew install mesa
```

#### Missing GLEW

**Linux:**
```bash
sudo apt-get install libglew-dev
```

**macOS:**
```bash
brew install glew
```

**Windows:**
GLEW should be available through vcpkg or downloaded manually.

#### X11 Libraries Not Found (Linux)

```bash
sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev \
                     libxcursor-dev libxi-dev libxext-dev libxtst-dev
```

#### GLFW Build Errors

If you encounter errors building GLFW, try:

```bash
cd build
rm -rf libs/glfw
cd ..
./setup_dependencies.sh  # Re-download GLFW
cd build
cmake .. && make
```

#### Permission Denied When Running

**Linux:**
```bash
chmod +x build/MouseTracker
./build/MouseTracker
```

#### Application Won't Start - Missing Libraries

Check required libraries:

**Linux:**
```bash
ldd build/MouseTracker
```

Install any missing libraries shown as "not found".

### Clean Build

To perform a clean build:

```bash
cd build
rm -rf *
cmake ..
make -j$(nproc)
```

### Debug Build

For debugging with symbols:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

## Advanced Build Options

### Custom Install Prefix

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/mousetracker
make
sudo make install
```

### Cross-Compilation

For cross-compiling (e.g., for Raspberry Pi):

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/toolchain.cmake
```

### Static Linking (Linux)

To create a more portable binary:

```bash
cmake .. -DBUILD_SHARED_LIBS=OFF
```

Note: Some libraries (like X11) will still be dynamically linked.

## Performance Optimization

### Compiler Optimization Flags

For maximum performance:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_CXX_FLAGS="-O3 -march=native"
make -j$(nproc)
```

### Link-Time Optimization (LTO)

```bash
cmake .. -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
```

## Continuous Integration

Example GitHub Actions workflow:

```yaml
name: Build

on: [push, pull_request]

jobs:
  build-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y libgl1-mesa-dev libx11-dev libglew-dev \
                                  libxrandr-dev libxinerama-dev libxcursor-dev \
                                  libxi-dev libxext-dev libxtst-dev
      - name: Setup dependencies
        run: ./setup_dependencies.sh
      - name: Build
        run: |
          mkdir build && cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release
          make -j$(nproc)
```

## Next Steps

After building:

1. **Run the Application**: `./build/MouseTracker`
2. **Configure Settings**: Modify `config.json` or use the UI
3. **Read the README**: See `README.md` for usage instructions
4. **Report Issues**: Open an issue on GitHub if you encounter problems

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [Dear ImGui Wiki](https://github.com/ocornut/imgui/wiki)
- [Project README](README.md)

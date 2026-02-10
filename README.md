# Mouse Tracker - Advanced Aim Assist Application

A full-featured C++ application with mouse tracking, FOV (Field of View) customization, and an elegant user interface built with ImGui.

## Features

### 🎯 Mouse Tracking
- Real-time mouse tracking for targets within FOV
- Adjustable tracking strength slider (from basic aim assist to full aim bot)
- Smooth, human-like tracking behavior with acceleration curves
- Configurable smoothness and maximum speed parameters

### 🔵 FOV Customization
- Customizable FOV circle size, transparency, and color
- Live preview functionality for FOV settings
- Visual crosshair at FOV center
- Real-time position and radius display

### 🎨 Elegant User Interface
- Modern, responsive UI with ImGui
- Tabbed interface for organized settings:
  - **General Settings**: Application configuration
  - **FOV Settings**: Field of view customization
  - **Tracking Settings**: Mouse tracking parameters
  - **Logs**: Real-time log viewer
- Dark and light mode support
- FPS counter for performance monitoring

### ⚙️ Advanced Settings
- JSON-based settings save/load functionality
- Reset button to restore default settings
- Settings persist between sessions
- Menu bar for quick access to common actions

### ⚡ High Performance
- Optimized for minimal resource usage
- Real-time FPS display
- V-Sync support for smooth rendering
- Efficient OpenGL rendering

### 🎬 Splash Screen
- Professional splash screen on application launch
- Customizable duration

### 🛡️ Error Handling
- Comprehensive error logging system
- User-friendly error messages in UI
- Log viewer with color-coded severity levels
- Persistent log file (mousetracker.log)

## Project Structure

```
C-/
├── CMakeLists.txt           # CMake build configuration
├── build.sh                 # Build script
├── setup.sh                 # Dependency setup script
├── README.md                # This file
├── .gitignore              # Git ignore rules
├── src/
│   ├── main.cpp            # Application entry point
│   ├── Application.h/cpp   # Main application class
│   ├── MouseTracker.h/cpp  # Mouse tracking implementation
│   ├── FOVRenderer.h/cpp   # FOV circle rendering
│   ├── Settings.h/cpp      # Settings management
│   ├── Logger.h/cpp        # Logging system
│   └── SplashScreen.h/cpp  # Splash screen
└── external/               # External dependencies (auto-downloaded)
    ├── imgui/             # Dear ImGui library
    └── json/              # nlohmann/json library
```

## Requirements

### System Dependencies
- **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake** 3.15 or higher
- **SDL2** development libraries
- **OpenGL** development libraries

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libsdl2-dev libgl1-mesa-dev
```

### Fedora
```bash
sudo dnf install gcc-c++ cmake git
sudo dnf install SDL2-devel mesa-libGL-devel
```

### Arch Linux
```bash
sudo pacman -S base-devel cmake git
sudo pacman -S sdl2 mesa
```

### macOS
```bash
brew install cmake sdl2
```

### Windows
- Install Visual Studio 2019 or later with C++ support
- Install CMake from https://cmake.org/
- Download SDL2 development libraries from https://www.libsdl.org/

## Building the Application

### Step 1: Setup Dependencies
```bash
./setup.sh
```

This script will:
- Download Dear ImGui library
- Download nlohmann/json library
- Set up the external dependencies

### Step 2: Build
```bash
./build.sh
```

This script will:
- Create a build directory
- Run CMake configuration
- Compile the application

### Manual Build (Alternative)
```bash
# Setup dependencies
./setup.sh

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build . -j$(nproc)

# Run
./MouseTracker
```

## Running the Application

After building, run:
```bash
./build/MouseTracker
```

Or from the build directory:
```bash
cd build
./MouseTracker
```

## Usage Guide

### General Settings Tab
- Toggle V-Sync on/off
- Show/hide FPS counter
- Switch between dark and light modes
- Adjust window dimensions

### FOV Settings Tab
1. **Show FOV Circle**: Toggle visibility of the FOV circle
2. **Radius**: Adjust the size of the FOV (50-500 pixels)
3. **Center Position**: Set X/Y coordinates for FOV center
4. **Color & Transparency**: Customize FOV appearance with RGBA color picker
5. **Live Preview**: See real-time updates of your FOV settings

### Tracking Settings Tab
1. **Enable Tracking**: Toggle mouse tracking on/off
2. **Strength Slider**: 
   - 0.0 = Basic aim assist (subtle corrections)
   - 1.0 = Full aim bot (aggressive tracking)
3. **Smoothness**: Control how smooth the tracking motion is (0.1-1.0)
4. **Max Speed**: Set maximum tracking speed in pixels/second
5. **Acceleration**: Adjust acceleration curve for human-like movement
6. **Status Display**: Shows current target position or "No Target"

### Logs Tab
- View all application logs in real-time
- Color-coded by severity (Info: white, Warning: yellow, Error: red)
- Clear logs button
- Auto-scroll to latest entries

### Menu Bar
- **File Menu**:
  - Save Settings: Persist current configuration
  - Load Settings: Restore saved configuration
  - Reset to Default: Restore factory settings
  - Exit: Close application
- **View Menu**:
  - Dark Mode toggle
  - Show FPS toggle

## Configuration

Settings are automatically saved to `settings.json` in the application directory. The file is created on first run and updated when you save settings.

### Default Settings
```json
{
  "tracking": {
    "enabled": false,
    "strength": 0.5,
    "smoothness": 0.8,
    "maxSpeed": 500.0,
    "accelerationCurve": 2.0
  },
  "fov": {
    "radius": 100.0,
    "centerX": 400.0,
    "centerY": 300.0,
    "colorR": 1.0,
    "colorG": 0.0,
    "colorB": 0.0,
    "colorA": 0.5,
    "visible": true,
    "segments": 64
  },
  "ui": {
    "darkMode": true,
    "showFPS": true
  },
  "general": {
    "windowWidth": 1280,
    "windowHeight": 720,
    "vsync": true
  }
}
```

## Logging

The application generates a log file `mousetracker.log` in the application directory. This file contains:
- Timestamps for all events
- Severity levels (INFO, WARNING, ERROR)
- Detailed error messages
- Application state changes

## Troubleshooting

### Build Issues

**Problem**: CMake cannot find SDL2
```bash
# Solution: Set SDL2 path manually
cmake -DSDL2_DIR=/path/to/sdl2 ..
```

**Problem**: Missing OpenGL headers
```bash
# Ubuntu/Debian
sudo apt-get install libgl1-mesa-dev

# Fedora
sudo dnf install mesa-libGL-devel
```

### Runtime Issues

**Problem**: Application crashes on startup
- Check `mousetracker.log` for error messages
- Ensure SDL2 runtime libraries are installed
- Verify OpenGL drivers are up to date

**Problem**: Low FPS
- Enable V-Sync in General Settings
- Check GPU driver support for OpenGL 3.0+
- Reduce FOV circle segments in settings

**Problem**: Settings not saving
- Ensure write permissions in application directory
- Check `mousetracker.log` for file access errors

## Performance Tips

1. **Enable V-Sync**: Prevents tearing and caps FPS at monitor refresh rate
2. **Reduce FOV Segments**: Lower segment count (32-64) for smoother performance
3. **Adjust Tracking Smoothness**: Higher smoothness values are more efficient
4. **Monitor FPS**: Use the FPS counter to identify performance issues

## Development

### Code Structure

- **Application.cpp**: Main application loop, event handling, and UI rendering
- **MouseTracker.cpp**: Implements tracking algorithms with smoothing and easing
- **FOVRenderer.cpp**: OpenGL-based circle rendering with transparency
- **Settings.cpp**: JSON serialization/deserialization
- **Logger.cpp**: Thread-safe logging with file and console output
- **SplashScreen.cpp**: Splash screen implementation

### Adding Features

To add new features:
1. Add settings to appropriate Settings struct
2. Implement UI controls in `Application::RenderUI()`
3. Add save/load support in `Settings.cpp`
4. Update this README with documentation

## License

This project is provided as-is for educational and development purposes.

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## Support

For issues, questions, or suggestions:
- Check the troubleshooting section
- Review `mousetracker.log` for error details
- Open an issue on the repository

## Acknowledgments

- **Dear ImGui**: Excellent immediate mode GUI library
- **SDL2**: Cross-platform multimedia library
- **nlohmann/json**: Modern JSON library for C++

---

**Note**: This application is intended for educational purposes and game development testing. Use responsibly and in accordance with applicable terms of service and laws.

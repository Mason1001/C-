# Mouse Tracker Application

A full-featured mouse tracking application with an elegant ImGui-based user interface, FOV customization, and advanced tracking capabilities.

## Features

### 🎯 Mouse Tracking
- **Adjustable Tracking Strength**: Slider to adjust from basic aim assist (0.0) to full aimbot behavior (1.0)
- **Smooth Tracking**: Human-like tracking behavior with adjustable smoothness
- **FOV-based Detection**: Only tracks targets within the defined Field of View

### 🔍 FOV Customization
- **Adjustable Size**: FOV circle radius from 10 to 500 pixels
- **Transparency Control**: Adjust FOV circle transparency
- **Color Customization**: Full RGBA color picker for the FOV circle
- **Live Preview**: Real-time preview of FOV settings

### 🎨 Elegant User Interface
- **Modern Design**: Clean, responsive UI built with Dear ImGui
- **Tab-based Organization**: Separate tabs for General, FOV, and Tracking settings
- **Dark/Light Mode**: Toggle between dark and light themes
- **FPS Counter**: Real-time FPS display for performance monitoring

### ⚙️ Advanced Settings
- **JSON Configuration**: Save and load settings in JSON format
- **Reset to Defaults**: One-click reset to default settings
- **Auto-save**: Settings automatically saved on exit

### 🚀 High Performance
- **Optimized Rendering**: Minimal resource usage
- **FPS Display**: Monitor application performance
- **Configurable Target FPS**: Set desired frame rate (30-144 FPS)

### 🌟 Additional Features
- **Splash Screen**: Beautiful splash screen on application launch
- **Comprehensive Logging**: Detailed error and info logging to file
- **Error Handling**: User-friendly error messages in the UI
- **Cross-platform**: Support for Windows and Linux

## File Structure

```
C-/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── setup_dependencies.sh       # Dependency setup script
├── .gitignore                  # Git ignore rules
│
├── include/                    # Header files
│   ├── Application.h           # Main application class
│   ├── Config.h                # Configuration management
│   ├── Logger.h                # Logging utility
│   ├── MouseTracker.h          # Mouse tracking logic
│   ├── SplashScreen.h          # Splash screen
│   └── UI.h                    # User interface
│
├── src/                        # Source files
│   ├── main.cpp                # Application entry point
│   ├── Application.cpp         # Application implementation
│   ├── Config.cpp              # Configuration implementation
│   ├── Logger.cpp              # Logger implementation
│   ├── MouseTracker.cpp        # Mouse tracker implementation
│   ├── SplashScreen.cpp        # Splash screen implementation
│   └── UI.cpp                  # UI implementation
│
├── libs/                       # External libraries (auto-downloaded)
│   ├── imgui/                  # Dear ImGui
│   ├── glfw/                   # GLFW window library
│   ├── glad/                   # OpenGL loader
│   └── nlohmann/               # JSON library
│
└── assets/                     # Application assets
    └── (logos, icons, etc.)
```

## Build Instructions

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git
sudo apt-get install -y libgl1-mesa-dev libx11-dev libxrandr-dev
sudo apt-get install -y libxinerama-dev libxcursor-dev libxi-dev
sudo apt-get install -y libxext-dev libxtst-dev
```

#### Windows
- Visual Studio 2019 or later (with C++ Desktop Development)
- CMake 3.15 or later
- Git

### Build Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/Mason1001/C-.git
   cd C-
   ```

2. **Setup dependencies**
   ```bash
   # On Linux/Mac
   chmod +x setup_dependencies.sh
   ./setup_dependencies.sh
   
   # On Windows (Git Bash or WSL)
   bash setup_dependencies.sh
   ```

3. **Build the project**
   ```bash
   mkdir build
   cd build
   cmake ..
   make -j$(nproc)  # Linux/Mac
   # OR
   cmake --build . --config Release  # Windows
   ```

4. **Run the application**
   ```bash
   ./MouseTracker  # Linux/Mac
   # OR
   Release\MouseTracker.exe  # Windows
   ```

## Usage

### Getting Started

1. **Launch the Application**: Run the executable. A splash screen will appear for 2.5 seconds.

2. **Navigate Tabs**: Use the tab bar to switch between different settings:
   - **General**: Application-wide settings (theme, FPS, etc.)
   - **FOV Settings**: Customize the Field of View circle
   - **Tracking**: Configure mouse tracking behavior

3. **Enable Tracking**: In the Tracking tab, check "Enable Tracking" to activate mouse tracking.

4. **Adjust Settings**: 
   - Use sliders to adjust tracking strength and smoothness
   - Customize FOV size, color, and transparency
   - Preview changes in real-time

5. **Save Settings**: Use File → Save Settings (Ctrl+S) to persist your configuration

### Keyboard Shortcuts

- `Ctrl+S`: Save settings
- `Ctrl+L`: Load settings
- `Alt+F4`: Exit application
- `ESC`: Exit application

### Configuration File

Settings are stored in `config.json` in the application directory:

```json
{
    "fov": {
        "radius": 100.0,
        "transparency": 0.5,
        "enabled": true,
        "color": [1.0, 0.0, 0.0, 1.0]
    },
    "tracking": {
        "enabled": false,
        "strength": 0.5,
        "smoothness": 0.7,
        "targetDetectionMode": 0
    },
    "general": {
        "darkMode": true,
        "showFPS": true,
        "showSplash": true,
        "targetFPS": 60
    }
}
```

## Logging

The application logs events to `app.log` in the application directory. Log levels include:
- **DEBUG**: Detailed debugging information
- **INFO**: General informational messages
- **WARNING**: Warning messages
- **ERROR**: Error messages
- **CRITICAL**: Critical errors that may cause the application to fail

## Error Handling

- All errors are logged to `app.log`
- User-friendly error messages are displayed in popup dialogs
- The application gracefully handles exceptions and provides meaningful feedback

## Performance

- **Target FPS**: Configurable from 30 to 144 FPS
- **Resource Usage**: Minimal CPU and memory footprint
- **Optimized Rendering**: Efficient ImGui rendering pipeline
- **FPS Monitoring**: Real-time FPS display in menu bar

## Important Notes

⚠️ **Educational Purpose Only**: This software is designed for educational purposes to demonstrate GUI programming, mouse tracking algorithms, and application architecture. Use of aim assist or aimbot features in games may violate terms of service.

⚠️ **Platform Limitations**: 
- Mouse tracking requires elevated permissions on some systems
- Linux requires X11 and XTest extension
- Windows requires user32.dll

## Troubleshooting

### Build Issues

**Problem**: CMake can't find OpenGL
```bash
# Linux
sudo apt-get install libgl1-mesa-dev

# Mac (with Homebrew)
brew install mesa
```

**Problem**: Missing dependencies during setup
```bash
# Re-run the dependency setup script
./setup_dependencies.sh
```

### Runtime Issues

**Problem**: Application won't start
- Check `app.log` for error messages
- Ensure all dependencies are installed
- Try running with elevated permissions (sudo on Linux)

**Problem**: Tracking not working
- Ensure tracking is enabled in the Tracking tab
- Check system permissions for mouse control
- On Linux, ensure XTest extension is available

**Problem**: Low FPS
- Adjust target FPS in General settings
- Disable VSync if needed
- Check system resource usage

## Contributing

Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is provided as-is for educational purposes.

## Credits

- **Dear ImGui**: [https://github.com/ocornut/imgui](https://github.com/ocornut/imgui)
- **GLFW**: [https://www.glfw.org/](https://www.glfw.org/)
- **GLAD**: [https://glad.dav1d.de/](https://glad.dav1d.de/)
- **nlohmann/json**: [https://github.com/nlohmann/json](https://github.com/nlohmann/json)

## Version History

### v1.0.0 (2026-02-03)
- Initial release
- Full-featured mouse tracking
- FOV customization
- Elegant ImGui-based UI
- Dark/Light theme support
- JSON configuration
- Splash screen
- Comprehensive logging and error handling

## Contact

For issues, questions, or suggestions, please open an issue on GitHub.

---

**Made with ❤️ for educational purposes**

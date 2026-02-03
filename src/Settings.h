#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include "MouseTracker.h"
#include "FOVRenderer.h"

class Settings {
public:
    Settings();
    ~Settings();

    bool Load(const std::string& filename);
    bool Save(const std::string& filename);
    void Reset();
    
    TrackingSettings trackingSettings;
    FOVSettings fovSettings;
    
    // UI Settings
    bool darkMode;
    bool showFPS;
    
    // General Settings
    int windowWidth;
    int windowHeight;
    bool vsync;
    
private:
    std::string m_Filename;
};

#endif // SETTINGS_H

#pragma once

#include <string>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

struct FOVSettings {
    float radius = 100.0f;
    float transparency = 0.5f;
    float color[4] = {1.0f, 0.0f, 0.0f, 1.0f}; // RGBA
    bool enabled = true;
};

struct TrackingSettings {
    bool enabled = false;
    float strength = 0.5f; // 0.0 = basic aim assist, 1.0 = full aimbot
    float smoothness = 0.7f;
    int targetDetectionMode = 0; // 0 = closest, 1 = crosshair
};

struct GeneralSettings {
    bool darkMode = true;
    bool showFPS = true;
    bool showSplash = true;
    int targetFPS = 60;
};

class Config {
public:
    static Config& getInstance();
    
    bool load(const std::string& filename = "config.json");
    bool save(const std::string& filename = "config.json");
    void reset();
    
    FOVSettings& getFOVSettings() { return fovSettings; }
    TrackingSettings& getTrackingSettings() { return trackingSettings; }
    GeneralSettings& getGeneralSettings() { return generalSettings; }
    
    const FOVSettings& getFOVSettings() const { return fovSettings; }
    const TrackingSettings& getTrackingSettings() const { return trackingSettings; }
    const GeneralSettings& getGeneralSettings() const { return generalSettings; }
    
private:
    Config();
    ~Config() = default;
    
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    void setDefaults();
    
    FOVSettings fovSettings;
    TrackingSettings trackingSettings;
    GeneralSettings generalSettings;
    
    std::string configFile;
};

#include "Config.h"
#include "Logger.h"
#include <iostream>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() : configFile("config.json") {
    setDefaults();
}

void Config::setDefaults() {
    // FOV Settings
    fovSettings.radius = 100.0f;
    fovSettings.transparency = 0.5f;
    fovSettings.color[0] = 1.0f; // R
    fovSettings.color[1] = 0.0f; // G
    fovSettings.color[2] = 0.0f; // B
    fovSettings.color[3] = 1.0f; // A
    fovSettings.enabled = true;
    
    // Tracking Settings
    trackingSettings.enabled = false;
    trackingSettings.strength = 0.5f;
    trackingSettings.smoothness = 0.7f;
    trackingSettings.targetDetectionMode = 0;
    
    // General Settings
    generalSettings.darkMode = true;
    generalSettings.showFPS = true;
    generalSettings.showSplash = true;
    generalSettings.targetFPS = 60;
}

bool Config::load(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            Logger::getInstance().warning("Config file not found, using defaults: " + filename);
            return false;
        }
        
        json j;
        file >> j;
        file.close();
        
        // Load FOV settings
        if (j.contains("fov")) {
            auto& fov = j["fov"];
            fovSettings.radius = fov.value("radius", 100.0f);
            fovSettings.transparency = fov.value("transparency", 0.5f);
            fovSettings.enabled = fov.value("enabled", true);
            
            if (fov.contains("color") && fov["color"].is_array() && fov["color"].size() >= 4) {
                for (int i = 0; i < 4; i++) {
                    fovSettings.color[i] = fov["color"][i];
                }
            }
        }
        
        // Load tracking settings
        if (j.contains("tracking")) {
            auto& tracking = j["tracking"];
            trackingSettings.enabled = tracking.value("enabled", false);
            trackingSettings.strength = tracking.value("strength", 0.5f);
            trackingSettings.smoothness = tracking.value("smoothness", 0.7f);
            trackingSettings.targetDetectionMode = tracking.value("targetDetectionMode", 0);
        }
        
        // Load general settings
        if (j.contains("general")) {
            auto& general = j["general"];
            generalSettings.darkMode = general.value("darkMode", true);
            generalSettings.showFPS = general.value("showFPS", true);
            generalSettings.showSplash = general.value("showSplash", true);
            generalSettings.targetFPS = general.value("targetFPS", 60);
        }
        
        Logger::getInstance().info("Configuration loaded successfully from " + filename);
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to load config: " + std::string(e.what()));
        return false;
    }
}

bool Config::save(const std::string& filename) {
    try {
        json j;
        
        // Save FOV settings
        j["fov"] = {
            {"radius", fovSettings.radius},
            {"transparency", fovSettings.transparency},
            {"enabled", fovSettings.enabled},
            {"color", {
                fovSettings.color[0],
                fovSettings.color[1],
                fovSettings.color[2],
                fovSettings.color[3]
            }}
        };
        
        // Save tracking settings
        j["tracking"] = {
            {"enabled", trackingSettings.enabled},
            {"strength", trackingSettings.strength},
            {"smoothness", trackingSettings.smoothness},
            {"targetDetectionMode", trackingSettings.targetDetectionMode}
        };
        
        // Save general settings
        j["general"] = {
            {"darkMode", generalSettings.darkMode},
            {"showFPS", generalSettings.showFPS},
            {"showSplash", generalSettings.showSplash},
            {"targetFPS", generalSettings.targetFPS}
        };
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            Logger::getInstance().error("Failed to open config file for writing: " + filename);
            return false;
        }
        
        file << j.dump(4);
        file.close();
        
        Logger::getInstance().info("Configuration saved successfully to " + filename);
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to save config: " + std::string(e.what()));
        return false;
    }
}

void Config::reset() {
    Logger::getInstance().info("Resetting configuration to defaults");
    setDefaults();
}

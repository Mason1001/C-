#include "Settings.h"
#include "Logger.h"
#include <fstream>
#include <sstream>

// Simple JSON parser/writer (simplified for this project)
// In production, you'd use nlohmann/json or similar

Settings::Settings()
    : m_Filename("settings.json")
{
    Reset();
}

Settings::~Settings() {
}

void Settings::Reset() {
    // Reset tracking settings
    trackingSettings = TrackingSettings();
    
    // Reset FOV settings
    fovSettings = FOVSettings();
    
    // Reset UI settings
    darkMode = true;
    showFPS = true;
    
    // Reset general settings
    windowWidth = 1280;
    windowHeight = 720;
    vsync = true;
    
    Logger::Instance().Info("Settings reset to defaults");
}

bool Settings::Save(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            Logger::Instance().Error("Failed to open settings file for writing: " + filename);
            return false;
        }
        
        // Write settings in JSON format
        file << "{\n";
        file << "  \"tracking\": {\n";
        file << "    \"enabled\": " << (trackingSettings.enabled ? "true" : "false") << ",\n";
        file << "    \"strength\": " << trackingSettings.strength << ",\n";
        file << "    \"smoothness\": " << trackingSettings.smoothness << ",\n";
        file << "    \"maxSpeed\": " << trackingSettings.maxSpeed << ",\n";
        file << "    \"accelerationCurve\": " << trackingSettings.accelerationCurve << "\n";
        file << "  },\n";
        file << "  \"fov\": {\n";
        file << "    \"radius\": " << fovSettings.radius << ",\n";
        file << "    \"centerX\": " << fovSettings.centerX << ",\n";
        file << "    \"centerY\": " << fovSettings.centerY << ",\n";
        file << "    \"colorR\": " << fovSettings.colorR << ",\n";
        file << "    \"colorG\": " << fovSettings.colorG << ",\n";
        file << "    \"colorB\": " << fovSettings.colorB << ",\n";
        file << "    \"colorA\": " << fovSettings.colorA << ",\n";
        file << "    \"visible\": " << (fovSettings.visible ? "true" : "false") << ",\n";
        file << "    \"segments\": " << fovSettings.segments << "\n";
        file << "  },\n";
        file << "  \"ui\": {\n";
        file << "    \"darkMode\": " << (darkMode ? "true" : "false") << ",\n";
        file << "    \"showFPS\": " << (showFPS ? "true" : "false") << "\n";
        file << "  },\n";
        file << "  \"general\": {\n";
        file << "    \"windowWidth\": " << windowWidth << ",\n";
        file << "    \"windowHeight\": " << windowHeight << ",\n";
        file << "    \"vsync\": " << (vsync ? "true" : "false") << "\n";
        file << "  }\n";
        file << "}\n";
        
        file.close();
        Logger::Instance().Info("Settings saved to: " + filename);
        return true;
    } catch (const std::exception& e) {
        Logger::Instance().Error(std::string("Error saving settings: ") + e.what());
        return false;
    }
}

bool Settings::Load(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            Logger::Instance().Warning("Settings file not found: " + filename + " (using defaults)");
            return false;
        }
        
        // Simple JSON parsing (simplified - in production use proper JSON library)
        std::string line;
        std::string section;
        
        while (std::getline(file, line)) {
            // Remove whitespace
            line.erase(0, line.find_first_not_of(" \t"));
            
            // Detect sections
            if (line.find("\"tracking\"") != std::string::npos) {
                section = "tracking";
            } else if (line.find("\"fov\"") != std::string::npos) {
                section = "fov";
            } else if (line.find("\"ui\"") != std::string::npos) {
                section = "ui";
            } else if (line.find("\"general\"") != std::string::npos) {
                section = "general";
            }
            
            // Parse values (simplified parsing)
            if (section == "tracking") {
                if (line.find("\"enabled\"") != std::string::npos) {
                    trackingSettings.enabled = line.find("true") != std::string::npos;
                } else if (line.find("\"strength\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        trackingSettings.strength = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"smoothness\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        trackingSettings.smoothness = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"maxSpeed\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        trackingSettings.maxSpeed = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"accelerationCurve\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        trackingSettings.accelerationCurve = std::stof(line.substr(pos + 1));
                    }
                }
            } else if (section == "fov") {
                if (line.find("\"radius\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.radius = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"centerX\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.centerX = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"centerY\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.centerY = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"colorR\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.colorR = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"colorG\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.colorG = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"colorB\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.colorB = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"colorA\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.colorA = std::stof(line.substr(pos + 1));
                    }
                } else if (line.find("\"visible\"") != std::string::npos) {
                    fovSettings.visible = line.find("true") != std::string::npos;
                } else if (line.find("\"segments\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        fovSettings.segments = std::stoi(line.substr(pos + 1));
                    }
                }
            } else if (section == "ui") {
                if (line.find("\"darkMode\"") != std::string::npos) {
                    darkMode = line.find("true") != std::string::npos;
                } else if (line.find("\"showFPS\"") != std::string::npos) {
                    showFPS = line.find("true") != std::string::npos;
                }
            } else if (section == "general") {
                if (line.find("\"windowWidth\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        windowWidth = std::stoi(line.substr(pos + 1));
                    }
                } else if (line.find("\"windowHeight\"") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos) {
                        windowHeight = std::stoi(line.substr(pos + 1));
                    }
                } else if (line.find("\"vsync\"") != std::string::npos) {
                    vsync = line.find("true") != std::string::npos;
                }
            }
        }
        
        file.close();
        Logger::Instance().Info("Settings loaded from: " + filename);
        return true;
    } catch (const std::exception& e) {
        Logger::Instance().Error(std::string("Error loading settings: ") + e.what());
        return false;
    }
}

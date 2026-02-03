#include "UI.h"
#include "Logger.h"
#include <cmath>

// Forward declaration
static void HelpMarker(const char* desc);

UI::UI() 
    : currentTab(0), currentFPS(0.0f), closeRequested(false), 
      showError(false), settingsChanged(false) {
    
    Config& config = Config::getInstance();
    applyTheme(config.getGeneralSettings().darkMode);
}

void UI::render(MouseTracker& tracker) {
    Config& config = Config::getInstance();
    
    renderMenuBar();
    
    // Main window
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - 20));
    
    ImGui::Begin("Mouse Tracker Control Panel", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse);
    
    // Tabs
    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("General")) {
            currentTab = 0;
            renderGeneralTab(tracker);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("FOV Settings")) {
            currentTab = 1;
            renderFOVTab(tracker);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Tracking")) {
            currentTab = 2;
            renderTrackingTab(tracker);
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
    
    // Error popup
    if (showError) {
        ImGui::OpenPopup("Error");
        if (ImGui::BeginPopupModal("Error", &showError, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", errorMessage.c_str());
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                showError = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void UI::renderMenuBar() {
    Config& config = Config::getInstance();
    
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Settings", "Ctrl+S")) {
                if (config.save()) {
                    Logger::getInstance().info("Settings saved");
                } else {
                    showErrorMessage("Failed to save settings");
                }
            }
            
            if (ImGui::MenuItem("Load Settings", "Ctrl+L")) {
                if (config.load()) {
                    Logger::getInstance().info("Settings loaded");
                } else {
                    showErrorMessage("Failed to load settings");
                }
            }
            
            if (ImGui::MenuItem("Reset to Defaults")) {
                config.reset();
                Logger::getInstance().info("Settings reset to defaults");
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                closeRequested = true;
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            bool darkMode = config.getGeneralSettings().darkMode;
            if (ImGui::MenuItem("Dark Mode", nullptr, &darkMode)) {
                config.getGeneralSettings().darkMode = darkMode;
                applyTheme(darkMode);
            }
            
            ImGui::MenuItem("Show FPS", nullptr, &config.getGeneralSettings().showFPS);
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                Logger::getInstance().info("Mouse Tracker v1.0.0");
            }
            ImGui::EndMenu();
        }
        
        // FPS counter on the right
        if (config.getGeneralSettings().showFPS) {
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 120);
            ImGui::Text("FPS: %.1f", currentFPS);
        }
        
        ImGui::EndMainMenuBar();
    }
}

void UI::renderGeneralTab(MouseTracker& tracker) {
    Config& config = Config::getInstance();
    auto& general = config.getGeneralSettings();
    
    ImGui::Spacing();
    ImGui::Text("General Application Settings");
    ImGui::Separator();
    ImGui::Spacing();
    
    if (ImGui::Checkbox("Dark Mode", &general.darkMode)) {
        applyTheme(general.darkMode);
    }
    ImGui::SameLine();
    HelpMarker("Toggle between dark and light theme");
    
    ImGui::Checkbox("Show FPS Counter", &general.showFPS);
    ImGui::SameLine();
    HelpMarker("Display frames per second in the menu bar");
    
    ImGui::Checkbox("Show Splash Screen", &general.showSplash);
    ImGui::SameLine();
    HelpMarker("Show splash screen on application startup");
    
    ImGui::Spacing();
    ImGui::SliderInt("Target FPS", &general.targetFPS, 30, 144);
    ImGui::SameLine();
    HelpMarker("Target frame rate for the application");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::Text("Application Info");
    ImGui::BulletText("Version: 1.0.0");
    ImGui::BulletText("Build: Release");
    ImGui::BulletText("Platform: %s", 
#ifdef _WIN32
        "Windows"
#elif __linux__
        "Linux"
#else
        "Unknown"
#endif
    );
}

void UI::renderFOVTab(MouseTracker& tracker) {
    Config& config = Config::getInstance();
    auto& fov = config.getFOVSettings();
    
    ImGui::Spacing();
    ImGui::Text("Field of View Settings");
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::Checkbox("Enable FOV Circle", &fov.enabled);
    ImGui::SameLine();
    HelpMarker("Show the FOV circle overlay");
    
    ImGui::Spacing();
    
    if (ImGui::SliderFloat("FOV Radius", &fov.radius, 10.0f, 500.0f, "%.0f px")) {
        tracker.setFOVRadius(fov.radius);
    }
    ImGui::SameLine();
    HelpMarker("Size of the field of view circle");
    
    ImGui::SliderFloat("Transparency", &fov.transparency, 0.0f, 1.0f, "%.2f");
    ImGui::SameLine();
    HelpMarker("Transparency of the FOV circle (0 = invisible, 1 = opaque)");
    
    ImGui::Spacing();
    ImGui::Text("FOV Circle Color");
    ImGui::ColorEdit4("Color", fov.color, ImGuiColorEditFlags_AlphaBar);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::Text("Live Preview");
    renderFOVPreview();
}

void UI::renderTrackingTab(MouseTracker& tracker) {
    Config& config = Config::getInstance();
    auto& tracking = config.getTrackingSettings();
    
    ImGui::Spacing();
    ImGui::Text("Mouse Tracking Settings");
    ImGui::Separator();
    ImGui::Spacing();
    
    if (ImGui::Checkbox("Enable Tracking", &tracking.enabled)) {
        tracker.setEnabled(tracking.enabled);
    }
    ImGui::SameLine();
    HelpMarker("Enable or disable mouse tracking");
    
    if (!tracking.enabled) {
        ImGui::BeginDisabled();
    }
    
    ImGui::Spacing();
    
    if (ImGui::SliderFloat("Tracking Strength", &tracking.strength, 0.0f, 1.0f, "%.2f")) {
        tracker.setStrength(tracking.strength);
    }
    ImGui::SameLine();
    HelpMarker("0.0 = Basic aim assist, 1.0 = Full aimbot behavior");
    
    // Visual indicator
    ImGui::ProgressBar(tracking.strength, ImVec2(-1, 0));
    if (tracking.strength < 0.3f) {
        ImGui::Text("Mode: Light Assist");
    } else if (tracking.strength < 0.7f) {
        ImGui::Text("Mode: Medium Assist");
    } else {
        ImGui::Text("Mode: Strong Assist");
    }
    
    ImGui::Spacing();
    
    if (ImGui::SliderFloat("Smoothness", &tracking.smoothness, 0.0f, 1.0f, "%.2f")) {
        tracker.setSmoothness(tracking.smoothness);
    }
    ImGui::SameLine();
    HelpMarker("Higher values = smoother, more human-like tracking");
    
    ImGui::Spacing();
    
    const char* modes[] = { "Closest to Center", "Closest to Crosshair" };
    ImGui::Combo("Target Detection", &tracking.targetDetectionMode, modes, IM_ARRAYSIZE(modes));
    ImGui::SameLine();
    HelpMarker("How targets are selected within the FOV");
    
    if (!tracking.enabled) {
        ImGui::EndDisabled();
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Warning:");
    ImGui::TextWrapped("Use of aim assist or aimbot features may violate game terms of service. "
                      "This software is for educational purposes only.");
}

void UI::renderFOVPreview() {
    Config& config = Config::getInstance();
    auto& fov = config.getFOVSettings();
    
    if (!fov.enabled) {
        ImGui::Text("FOV Preview (Disabled)");
        return;
    }
    
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    canvas_size.y = 300.0f; // Fixed height for preview
    
    ImGui::InvisibleButton("canvas", canvas_size);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Draw background
    draw_list->AddRectFilled(canvas_pos, 
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(50, 50, 50, 255));
    
    // Draw FOV circle
    ImVec2 center(canvas_pos.x + canvas_size.x * 0.5f, canvas_pos.y + canvas_size.y * 0.5f);
    float preview_radius = std::min(fov.radius * 0.5f, std::min(canvas_size.x, canvas_size.y) * 0.4f);
    
    ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(
        fov.color[0], 
        fov.color[1], 
        fov.color[2], 
        fov.color[3] * fov.transparency
    ));
    
    draw_list->AddCircle(center, preview_radius, color, 64, 2.0f);
    
    // Draw crosshair
    draw_list->AddLine(
        ImVec2(center.x - 10, center.y),
        ImVec2(center.x + 10, center.y),
        IM_COL32(255, 255, 255, 200), 1.0f);
    draw_list->AddLine(
        ImVec2(center.x, center.y - 10),
        ImVec2(center.x, center.y + 10),
        IM_COL32(255, 255, 255, 200), 1.0f);
}

void UI::applyTheme(bool darkMode) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    if (darkMode) {
        ImGui::StyleColorsDark();
        
        // Customize dark theme
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    } else {
        ImGui::StyleColorsLight();
        
        // Customize light theme
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
    }
    
    // Common style settings
    style.WindowRounding = 0.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
}

void UI::showErrorMessage(const std::string& message) {
    errorMessage = message;
    showError = true;
    Logger::getInstance().error(message);
}

void UI::setFPS(float fps) {
    currentFPS = fps;
}

// Helper function for tooltips
static void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

#pragma once

#include "imgui.h"
#include "Config.h"
#include "MouseTracker.h"
#include <string>

class UI {
public:
    UI();
    ~UI() = default;
    
    void render(MouseTracker& tracker);
    void setFPS(float fps);
    
    bool shouldClose() const { return closeRequested; }
    
private:
    void renderMenuBar();
    void renderGeneralTab(MouseTracker& tracker);
    void renderFOVTab(MouseTracker& tracker);
    void renderTrackingTab(MouseTracker& tracker);
    void renderFOVPreview();
    
    void applyTheme(bool darkMode);
    void showErrorMessage(const std::string& message);
    
    int currentTab;
    float currentFPS;
    bool closeRequested;
    
    std::string errorMessage;
    bool showError;
    
    // UI State
    bool settingsChanged;
};

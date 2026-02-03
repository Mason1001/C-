#include "SplashScreen.h"
#include "Config.h"
#include <chrono>

SplashScreen::SplashScreen() 
    : complete(false), displayTime(2.5f), currentTime(0.0f), fadeAlpha(0.0f) {
}

void SplashScreen::show() {
    complete = false;
    currentTime = 0.0f;
}

void SplashScreen::update() {
    if (complete) {
        return;
    }
    
    // Update timing
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTimePoint = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTimePoint - lastTime).count();
    lastTime = currentTimePoint;
    
    currentTime += deltaTime;
    
    // Calculate fade alpha
    if (currentTime < 0.5f) {
        // Fade in
        fadeAlpha = currentTime / 0.5f;
    } else if (currentTime < displayTime - 0.5f) {
        // Full display
        fadeAlpha = 1.0f;
    } else if (currentTime < displayTime) {
        // Fade out
        fadeAlpha = (displayTime - currentTime) / 0.5f;
    } else {
        // Complete
        fadeAlpha = 0.0f;
        complete = true;
    }
    
    render();
}

void SplashScreen::render() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, fadeAlpha));
    
    ImGui::Begin("SplashScreen", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar);
    
    // Center content
    ImVec2 windowSize = ImGui::GetWindowSize();
    
    // Draw title
    const char* title = "MOUSE TRACKER";
    ImGui::PushFont(io.Fonts->Fonts[0]); // Use default font, might want to make larger
    ImVec2 textSize = ImGui::CalcTextSize(title);
    ImGui::SetCursorPos(ImVec2((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f - 50));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, fadeAlpha));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();
    ImGui::PopFont();
    
    // Draw version
    const char* version = "v1.0.0";
    textSize = ImGui::CalcTextSize(version);
    ImGui::SetCursorPos(ImVec2((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, fadeAlpha));
    ImGui::Text("%s", version);
    ImGui::PopStyleColor();
    
    // Draw loading text
    const char* loading = "Loading...";
    textSize = ImGui::CalcTextSize(loading);
    ImGui::SetCursorPos(ImVec2((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f + 50));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, fadeAlpha * 0.7f));
    ImGui::Text("%s", loading);
    ImGui::PopStyleColor();
    
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

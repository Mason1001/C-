#pragma once

#include "imgui.h"

class SplashScreen {
public:
    SplashScreen();
    ~SplashScreen() = default;
    
    void show();
    bool isComplete() const { return complete; }
    void update();
    
private:
    bool complete;
    float displayTime;
    float currentTime;
    float fadeAlpha;
    
    void render();
};

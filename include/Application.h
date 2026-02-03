#pragma once

#include <string>
#include "UI.h"
#include "MouseTracker.h"
#include "SplashScreen.h"

struct GLFWwindow;

class Application {
public:
    Application();
    ~Application();
    
    bool initialize();
    void run();
    void shutdown();
    
private:
    bool initWindow();
    bool initImGui();
    void processInput();
    void update(float deltaTime);
    void render();
    void cleanup();
    
    GLFWwindow* window;
    UI* ui;
    MouseTracker* tracker;
    SplashScreen* splashScreen;
    
    bool running;
    bool initialized;
    
    float lastFrameTime;
    float deltaTime;
    float fps;
    int frameCount;
    float fpsTimer;
    
    std::string lastError;
};

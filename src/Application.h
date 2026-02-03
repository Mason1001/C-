#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL2/SDL.h>
#include <memory>
#include <string>

class MouseTracker;
class FOVRenderer;
class Settings;
class Logger;

class Application {
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
    void RenderUI();
    void ShowSplashScreen();

    SDL_Window* m_Window;
    SDL_GLContext m_GLContext;
    
    std::unique_ptr<MouseTracker> m_MouseTracker;
    std::unique_ptr<FOVRenderer> m_FOVRenderer;
    std::unique_ptr<Settings> m_Settings;
    std::unique_ptr<Logger> m_Logger;

    bool m_Running;
    bool m_ShowSplash;
    float m_SplashTimer;
    
    // UI state
    bool m_DarkMode;
    int m_CurrentTab;
    
    // Performance monitoring
    float m_FPS;
    float m_FrameTime;
};

#endif // APPLICATION_H

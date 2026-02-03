#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <SDL2/SDL.h>
#include <string>

class SplashScreen {
public:
    SplashScreen();
    ~SplashScreen();

    bool Show(SDL_Window* parentWindow, float duration);
    void Render();
    bool IsFinished() const { return m_Finished; }
    
private:
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    bool m_Finished;
    float m_Timer;
    float m_Duration;
};

#endif // SPLASH_SCREEN_H

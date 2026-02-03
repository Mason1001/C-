#include "SplashScreen.h"
#include "Logger.h"

SplashScreen::SplashScreen()
    : m_Window(nullptr)
    , m_Renderer(nullptr)
    , m_Finished(false)
    , m_Timer(0.0f)
    , m_Duration(2.0f)
{
}

SplashScreen::~SplashScreen() {
    if (m_Renderer) {
        SDL_DestroyRenderer(m_Renderer);
    }
    if (m_Window) {
        SDL_DestroyWindow(m_Window);
    }
}

bool SplashScreen::Show(SDL_Window* parentWindow, float duration) {
    m_Duration = duration;
    
    // Create splash window
    m_Window = SDL_CreateWindow(
        "Mouse Tracker",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        400,
        200,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );
    
    if (!m_Window) {
        Logger::Instance().Error("Failed to create splash window");
        return false;
    }
    
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_Renderer) {
        Logger::Instance().Error("Failed to create splash renderer");
        return false;
    }
    
    Render();
    
    return true;
}

void SplashScreen::Render() {
    if (!m_Renderer) return;
    
    // Clear with dark background
    SDL_SetRenderDrawColor(m_Renderer, 20, 20, 30, 255);
    SDL_RenderClear(m_Renderer);
    
    // TODO: In a real implementation, you'd load and render an image here
    // For now, just show a colored rectangle
    SDL_Rect rect = {50, 50, 300, 100};
    SDL_SetRenderDrawColor(m_Renderer, 100, 150, 255, 255);
    SDL_RenderFillRect(m_Renderer, &rect);
    
    SDL_RenderPresent(m_Renderer);
}

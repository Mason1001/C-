#ifndef FOV_RENDERER_H
#define FOV_RENDERER_H

#include <SDL2/SDL.h>

struct FOVSettings {
    float radius;
    float centerX;
    float centerY;
    float colorR, colorG, colorB, colorA;
    bool visible;
    int segments;
    
    FOVSettings()
        : radius(100.0f)
        , centerX(400.0f)
        , centerY(300.0f)
        , colorR(1.0f)
        , colorG(0.0f)
        , colorB(0.0f)
        , colorA(0.5f)
        , visible(true)
        , segments(64) {}
};

class FOVRenderer {
public:
    FOVRenderer();
    ~FOVRenderer();

    void Initialize();
    void Render();
    
    void SetSettings(const FOVSettings& settings) { m_Settings = settings; }
    FOVSettings& GetSettings() { return m_Settings; }
    
    bool IsPointInFOV(float x, float y) const;
    
private:
    void DrawCircle(float centerX, float centerY, float radius, 
                   float r, float g, float b, float a, int segments);

    FOVSettings m_Settings;
};

#endif // FOV_RENDERER_H

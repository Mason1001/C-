#ifndef MOUSE_TRACKER_H
#define MOUSE_TRACKER_H

#include <vector>
#include <utility>

struct TrackingSettings {
    bool enabled;
    float strength;           // 0.0 = basic assist, 1.0 = full aim bot
    float smoothness;         // Higher values = smoother movement
    float maxSpeed;           // Maximum tracking speed
    float accelerationCurve;  // Acceleration curve for human-like movement
    
    TrackingSettings()
        : enabled(false)
        , strength(0.5f)
        , smoothness(0.8f)
        , maxSpeed(500.0f)
        , accelerationCurve(2.0f) {}
};

class MouseTracker {
public:
    MouseTracker();
    ~MouseTracker();

    void Update(float deltaTime);
    void SetTarget(float x, float y);
    bool HasTarget() const { return m_HasTarget; }
    void ClearTarget() { m_HasTarget = false; }
    
    void SetSettings(const TrackingSettings& settings) { m_Settings = settings; }
    TrackingSettings& GetSettings() { return m_Settings; }
    
    std::pair<float, float> GetCurrentPosition() const { return {m_CurrentX, m_CurrentY}; }
    std::pair<float, float> GetTargetPosition() const { return {m_TargetX, m_TargetY}; }

private:
    void ApplySmoothing(float& deltaX, float& deltaY, float deltaTime);
    float CalculateEasing(float t) const;

    TrackingSettings m_Settings;
    
    float m_CurrentX, m_CurrentY;
    float m_TargetX, m_TargetY;
    float m_VelocityX, m_VelocityY;
    bool m_HasTarget;
};

#endif // MOUSE_TRACKER_H

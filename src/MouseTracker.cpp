#include "MouseTracker.h"
#include "Logger.h"
#include <cmath>
#include <algorithm>

MouseTracker::MouseTracker()
    : m_CurrentX(0.0f)
    , m_CurrentY(0.0f)
    , m_TargetX(0.0f)
    , m_TargetY(0.0f)
    , m_VelocityX(0.0f)
    , m_VelocityY(0.0f)
    , m_HasTarget(false)
{
    Logger::Instance().Info("MouseTracker initialized");
}

MouseTracker::~MouseTracker() {
}

void MouseTracker::SetTarget(float x, float y) {
    m_TargetX = x;
    m_TargetY = y;
    m_HasTarget = true;
}

void MouseTracker::Update(float deltaTime) {
    if (!m_Settings.enabled || !m_HasTarget) {
        return;
    }
    
    // Calculate distance to target
    float deltaX = m_TargetX - m_CurrentX;
    float deltaY = m_TargetY - m_CurrentY;
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    
    if (distance < 1.0f) {
        return; // Already at target
    }
    
    // Normalize direction
    float dirX = deltaX / distance;
    float dirY = deltaY / distance;
    
    // Apply tracking strength
    float effectiveStrength = m_Settings.strength;
    
    // Calculate desired velocity based on strength
    float desiredSpeed = std::min(distance * effectiveStrength, m_Settings.maxSpeed);
    float desiredVelX = dirX * desiredSpeed;
    float desiredVelY = dirY * desiredSpeed;
    
    // Apply smoothing (lerp towards desired velocity)
    float smoothFactor = std::min(1.0f, deltaTime * (1.0f - m_Settings.smoothness) * 10.0f);
    m_VelocityX += (desiredVelX - m_VelocityX) * smoothFactor;
    m_VelocityY += (desiredVelY - m_VelocityY) * smoothFactor;
    
    // Apply acceleration curve for human-like movement
    float speed = std::sqrt(m_VelocityX * m_VelocityX + m_VelocityY * m_VelocityY);
    float normalizedSpeed = std::min(1.0f, speed / m_Settings.maxSpeed);
    float easedSpeed = CalculateEasing(normalizedSpeed);
    
    if (speed > 0.0f) {
        float scaleFactor = (easedSpeed * m_Settings.maxSpeed) / speed;
        m_VelocityX *= scaleFactor;
        m_VelocityY *= scaleFactor;
    }
    
    // Update position
    m_CurrentX += m_VelocityX * deltaTime;
    m_CurrentY += m_VelocityY * deltaTime;
    
    // Add slight random jitter for human-like behavior (very small)
    if (m_Settings.strength < 0.95f) { // Don't add jitter at max strength
        float jitterX = (std::rand() % 100 - 50) * 0.001f * (1.0f - m_Settings.strength);
        float jitterY = (std::rand() % 100 - 50) * 0.001f * (1.0f - m_Settings.strength);
        m_CurrentX += jitterX;
        m_CurrentY += jitterY;
    }
}

void MouseTracker::ApplySmoothing(float& deltaX, float& deltaY, float deltaTime) {
    // Apply exponential smoothing
    float alpha = 1.0f - std::exp(-deltaTime * m_Settings.smoothness);
    deltaX *= alpha;
    deltaY *= alpha;
}

float MouseTracker::CalculateEasing(float t) const {
    // Apply acceleration curve using power function
    // This creates more human-like acceleration
    return std::pow(t, m_Settings.accelerationCurve);
}

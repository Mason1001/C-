#pragma once

#include <vector>
#include <cmath>

struct Point2D {
    float x, y;
    
    Point2D(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
    
    float distance(const Point2D& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

struct Target {
    Point2D position;
    bool valid;
    
    Target() : valid(false) {}
    Target(float x, float y) : position(x, y), valid(true) {}
};

class MouseTracker {
public:
    MouseTracker();
    ~MouseTracker() = default;
    
    void update(float deltaTime);
    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled; }
    
    void setStrength(float strength);
    float getStrength() const { return trackingStrength; }
    
    void setSmoothness(float smoothness);
    float getSmoothness() const { return trackingSmoothness; }
    
    void setFOVRadius(float radius);
    float getFOVRadius() const { return fovRadius; }
    
    Point2D getCurrentMousePosition() const;
    Point2D getScreenCenter() const;
    
    Target findTarget();
    void applyTracking(const Target& target, float deltaTime);
    
    bool isTargetInFOV(const Point2D& target) const;
    
private:
    bool enabled;
    float trackingStrength;
    float trackingSmoothness;
    float fovRadius;
    
    Point2D lastMousePos;
    Point2D smoothedTarget;
    
    void moveMouse(float dx, float dy);
    Point2D interpolate(const Point2D& from, const Point2D& to, float t);
    
    std::vector<Target> detectTargets();
};

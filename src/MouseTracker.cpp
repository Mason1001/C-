#include "MouseTracker.h"
#include "Config.h"
#include "Logger.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

MouseTracker::MouseTracker() 
    : enabled(false), trackingStrength(0.5f), trackingSmoothness(0.7f), fovRadius(100.0f),
      lastMousePos(0, 0), smoothedTarget(0, 0) {
    Logger::getInstance().info("MouseTracker initialized");
}

void MouseTracker::update(float deltaTime) {
    if (!enabled) {
        return;
    }
    
    try {
        Target target = findTarget();
        if (target.valid) {
            applyTracking(target, deltaTime);
        }
    } catch (const std::exception& e) {
        Logger::getInstance().error("MouseTracker update error: " + std::string(e.what()));
    }
}

void MouseTracker::setEnabled(bool en) {
    enabled = en;
    if (enabled) {
        Logger::getInstance().info("Mouse tracking enabled");
    } else {
        Logger::getInstance().info("Mouse tracking disabled");
    }
}

void MouseTracker::setStrength(float strength) {
    trackingStrength = std::max(0.0f, std::min(1.0f, strength));
}

void MouseTracker::setSmoothness(float smoothness) {
    trackingSmoothness = std::max(0.0f, std::min(1.0f, smoothness));
}

void MouseTracker::setFOVRadius(float radius) {
    fovRadius = std::max(10.0f, std::min(500.0f, radius));
}

Point2D MouseTracker::getCurrentMousePosition() const {
#ifdef _WIN32
    POINT pt;
    if (GetCursorPos(&pt)) {
        return Point2D(static_cast<float>(pt.x), static_cast<float>(pt.y));
    }
#elif __linux__
    Display* display = XOpenDisplay(nullptr);
    if (display) {
        Window root = DefaultRootWindow(display);
        Window root_return, child_return;
        int root_x, root_y, win_x, win_y;
        unsigned int mask_return;
        
        if (XQueryPointer(display, root, &root_return, &child_return,
                         &root_x, &root_y, &win_x, &win_y, &mask_return)) {
            XCloseDisplay(display);
            return Point2D(static_cast<float>(root_x), static_cast<float>(root_y));
        }
        XCloseDisplay(display);
    }
#endif
    return Point2D(0, 0);
}

Point2D MouseTracker::getScreenCenter() const {
#ifdef _WIN32
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    return Point2D(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));
#elif __linux__
    Display* display = XOpenDisplay(nullptr);
    if (display) {
        Screen* screen = DefaultScreenOfDisplay(display);
        int screenWidth = WidthOfScreen(screen);
        int screenHeight = HeightOfScreen(screen);
        XCloseDisplay(display);
        return Point2D(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));
    }
#endif
    return Point2D(960, 540); // Default 1920x1080 center
}

bool MouseTracker::isTargetInFOV(const Point2D& target) const {
    Point2D center = getScreenCenter();
    float distance = center.distance(target);
    return distance <= fovRadius;
}

Target MouseTracker::findTarget() {
    // This is a simulation - in a real implementation, you would use
    // computer vision or game-specific methods to detect targets
    
    // For demonstration, we simulate a target near the center
    Point2D center = getScreenCenter();
    Point2D currentMouse = getCurrentMousePosition();
    
    // Simulate a target slightly offset from center
    Point2D simulatedTarget(center.x + 30, center.y - 20);
    
    if (isTargetInFOV(simulatedTarget)) {
        return Target(simulatedTarget.x, simulatedTarget.y);
    }
    
    return Target(); // No valid target
}

void MouseTracker::applyTracking(const Target& target, float deltaTime) {
    if (!target.valid) {
        return;
    }
    
    Point2D currentPos = getCurrentMousePosition();
    
    // Calculate direction to target
    float dx = target.position.x - currentPos.x;
    float dy = target.position.y - currentPos.y;
    
    // Apply smoothing
    smoothedTarget = interpolate(smoothedTarget, target.position, 1.0f - trackingSmoothness);
    
    // Calculate movement based on strength
    float moveX = dx * trackingStrength * deltaTime * 10.0f;
    float moveY = dy * trackingStrength * deltaTime * 10.0f;
    
    // Apply smooth interpolation
    moveX *= (1.0f - trackingSmoothness);
    moveY *= (1.0f - trackingSmoothness);
    
    moveMouse(moveX, moveY);
}

void MouseTracker::moveMouse(float dx, float dy) {
#ifdef _WIN32
    POINT pt;
    if (GetCursorPos(&pt)) {
        SetCursorPos(pt.x + static_cast<int>(dx), pt.y + static_cast<int>(dy));
    }
#elif __linux__
    Display* display = XOpenDisplay(nullptr);
    if (display) {
        XTestFakeRelativeMotionEvent(display, static_cast<int>(dx), static_cast<int>(dy), CurrentTime);
        XFlush(display);
        XCloseDisplay(display);
    }
#endif
}

Point2D MouseTracker::interpolate(const Point2D& from, const Point2D& to, float t) {
    return Point2D(
        from.x + (to.x - from.x) * t,
        from.y + (to.y - from.y) * t
    );
}

std::vector<Target> MouseTracker::detectTargets() {
    // Placeholder for target detection
    // In a real implementation, this would use computer vision or game-specific APIs
    std::vector<Target> targets;
    return targets;
}

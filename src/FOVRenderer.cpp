#include "FOVRenderer.h"
#include "Logger.h"
#include <GL/gl.h>
#include <cmath>

FOVRenderer::FOVRenderer() {
}

FOVRenderer::~FOVRenderer() {
}

void FOVRenderer::Initialize() {
    Logger::Instance().Info("FOVRenderer initialized");
}

bool FOVRenderer::IsPointInFOV(float x, float y) const {
    float dx = x - m_Settings.centerX;
    float dy = y - m_Settings.centerY;
    float distanceSquared = dx * dx + dy * dy;
    return distanceSquared <= (m_Settings.radius * m_Settings.radius);
}

void FOVRenderer::Render() {
    if (!m_Settings.visible) {
        return;
    }
    
    DrawCircle(m_Settings.centerX, m_Settings.centerY, m_Settings.radius,
              m_Settings.colorR, m_Settings.colorG, m_Settings.colorB, m_Settings.colorA,
              m_Settings.segments);
}

void FOVRenderer::DrawCircle(float centerX, float centerY, float radius,
                             float r, float g, float b, float a, int segments) {
    // Setup OpenGL state for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1280, 720, 0, -1, 1); // Match window coordinates
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Draw circle outline
    glColor4f(r, g, b, a);
    glLineWidth(2.0f);
    
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    // Draw crosshair at center
    glBegin(GL_LINES);
    glVertex2f(centerX - 10, centerY);
    glVertex2f(centerX + 10, centerY);
    glVertex2f(centerX, centerY - 10);
    glVertex2f(centerX, centerY + 10);
    glEnd();
    
    glDisable(GL_BLEND);
    
    // Restore matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

#include "Application.h"
#include "MouseTracker.h"
#include "FOVRenderer.h"
#include "Settings.h"
#include "Logger.h"
#include "SplashScreen.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <iostream>

Application::Application()
    : m_Window(nullptr)
    , m_GLContext(nullptr)
    , m_Running(false)
    , m_ShowSplash(true)
    , m_SplashTimer(0.0f)
    , m_DarkMode(true)
    , m_CurrentTab(0)
    , m_FPS(0.0f)
    , m_FrameTime(0.0f)
{
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    Logger::Instance().Info("Initializing application...");
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        Logger::Instance().Error(std::string("SDL_Init Error: ") + SDL_GetError());
        return false;
    }
    
    // GL 3.0 + GLSL 130
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    // Create window
    m_Window = SDL_CreateWindow(
        "Mouse Tracker - Advanced Aim Assist",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    
    if (!m_Window) {
        Logger::Instance().Error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
        return false;
    }
    
    m_GLContext = SDL_GL_CreateContext(m_Window);
    SDL_GL_MakeCurrent(m_Window, m_GLContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_GLContext);
    ImGui_ImplOpenGL3_Init("#version 130");
    
    // Initialize components
    m_Settings = std::make_unique<Settings>();
    m_Settings->Load("settings.json");
    
    m_MouseTracker = std::make_unique<MouseTracker>();
    m_MouseTracker->SetSettings(m_Settings->trackingSettings);
    
    m_FOVRenderer = std::make_unique<FOVRenderer>();
    m_FOVRenderer->SetSettings(m_Settings->fovSettings);
    m_FOVRenderer->Initialize();
    
    m_DarkMode = m_Settings->darkMode;
    
    Logger::Instance().Info("Application initialized successfully");
    m_Running = true;
    
    return true;
}

void Application::Run() {
    Logger::Instance().Info("Application started");
    
    // Show splash screen
    if (m_ShowSplash) {
        ShowSplashScreen();
    }
    
    Uint64 lastTime = SDL_GetPerformanceCounter();
    const Uint64 frequency = SDL_GetPerformanceFrequency();
    
    while (m_Running) {
        // Calculate delta time
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (currentTime - lastTime) / (float)frequency;
        lastTime = currentTime;
        
        // Calculate FPS
        m_FrameTime = deltaTime;
        m_FPS = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
        
        ProcessEvents();
        Update(deltaTime);
        Render();
    }
}

void Application::ShowSplashScreen() {
    Logger::Instance().Info("Showing splash screen...");
    
    SDL_Delay(2000); // Show splash for 2 seconds
    m_ShowSplash = false;
    
    Logger::Instance().Info("Splash screen closed");
}

void Application::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        
        if (event.type == SDL_QUIT) {
            m_Running = false;
        }
        
        if (event.type == SDL_WINDOWEVENT && 
            event.window.event == SDL_WINDOWEVENT_CLOSE) {
            m_Running = false;
        }
        
        // Mouse movement for tracking
        if (event.type == SDL_MOUSEMOTION) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            
            // Check if mouse is in FOV
            if (m_FOVRenderer->IsPointInFOV((float)mouseX, (float)mouseY)) {
                m_MouseTracker->SetTarget((float)mouseX, (float)mouseY);
            } else {
                m_MouseTracker->ClearTarget();
            }
        }
    }
}

void Application::Update(float deltaTime) {
    m_MouseTracker->Update(deltaTime);
}

void Application::Render() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    // Apply theme
    if (m_DarkMode) {
        ImGui::StyleColorsDark();
    } else {
        ImGui::StyleColorsLight();
    }
    
    // Render UI
    RenderUI();
    
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render FOV circle
    if (m_Settings->fovSettings.visible) {
        m_FOVRenderer->Render();
    }
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_Window);
}

void Application::RenderUI() {
    // Main window
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Mouse Tracker Control Panel", nullptr, ImGuiWindowFlags_MenuBar)) {
        // Menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save Settings")) {
                    if (m_Settings->Save("settings.json")) {
                        Logger::Instance().Info("Settings saved successfully");
                    } else {
                        Logger::Instance().Error("Failed to save settings");
                    }
                }
                if (ImGui::MenuItem("Load Settings")) {
                    if (m_Settings->Load("settings.json")) {
                        Logger::Instance().Info("Settings loaded successfully");
                        m_MouseTracker->SetSettings(m_Settings->trackingSettings);
                        m_FOVRenderer->SetSettings(m_Settings->fovSettings);
                    } else {
                        Logger::Instance().Warning("Failed to load settings, using defaults");
                    }
                }
                if (ImGui::MenuItem("Reset to Default")) {
                    m_Settings->Reset();
                    m_MouseTracker->SetSettings(m_Settings->trackingSettings);
                    m_FOVRenderer->SetSettings(m_Settings->fovSettings);
                    Logger::Instance().Info("Settings reset to default");
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    m_Running = false;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Dark Mode", nullptr, &m_DarkMode);
                m_Settings->darkMode = m_DarkMode;
                ImGui::MenuItem("Show FPS", nullptr, &m_Settings->showFPS);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        // FPS Display
        if (m_Settings->showFPS) {
            ImGui::Text("FPS: %.1f (%.3f ms)", m_FPS, m_FrameTime * 1000.0f);
            ImGui::Separator();
        }
        
        // Tab bar
        if (ImGui::BeginTabBar("MainTabs")) {
            if (ImGui::BeginTabItem("General Settings")) {
                ImGui::Spacing();
                ImGui::Text("Application Settings");
                ImGui::Separator();
                
                ImGui::Checkbox("Enable V-Sync", &m_Settings->vsync);
                ImGui::Checkbox("Show FPS Counter", &m_Settings->showFPS);
                ImGui::Checkbox("Dark Mode", &m_DarkMode);
                m_Settings->darkMode = m_DarkMode;
                
                ImGui::Spacing();
                ImGui::Text("Window Size");
                ImGui::SliderInt("Width", &m_Settings->windowWidth, 800, 1920);
                ImGui::SliderInt("Height", &m_Settings->windowHeight, 600, 1080);
                
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("FOV Settings")) {
                ImGui::Spacing();
                ImGui::Text("Field of View Configuration");
                ImGui::Separator();
                
                FOVSettings& fov = m_Settings->fovSettings;
                
                ImGui::Checkbox("Show FOV Circle", &fov.visible);
                ImGui::SliderFloat("Radius", &fov.radius, 50.0f, 500.0f, "%.0f px");
                ImGui::SliderFloat("Center X", &fov.centerX, 0.0f, 1280.0f, "%.0f px");
                ImGui::SliderFloat("Center Y", &fov.centerY, 0.0f, 720.0f, "%.0f px");
                
                ImGui::Spacing();
                ImGui::Text("Color & Transparency");
                ImGui::ColorEdit4("FOV Color", &fov.colorR);
                
                ImGui::Spacing();
                ImGui::Text("Live Preview:");
                ImGui::Text("  Radius: %.0f px", fov.radius);
                ImGui::Text("  Position: (%.0f, %.0f)", fov.centerX, fov.centerY);
                
                m_FOVRenderer->SetSettings(fov);
                
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Tracking Settings")) {
                ImGui::Spacing();
                ImGui::Text("Mouse Tracking Configuration");
                ImGui::Separator();
                
                TrackingSettings& tracking = m_Settings->trackingSettings;
                
                ImGui::Checkbox("Enable Tracking", &tracking.enabled);
                
                ImGui::Spacing();
                ImGui::Text("Tracking Strength");
                ImGui::SliderFloat("Strength", &tracking.strength, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("0.0 = Basic Aim Assist\n1.0 = Full Aim Bot");
                }
                
                ImGui::Spacing();
                ImGui::Text("Movement Parameters");
                ImGui::SliderFloat("Smoothness", &tracking.smoothness, 0.1f, 1.0f, "%.2f");
                ImGui::SliderFloat("Max Speed", &tracking.maxSpeed, 100.0f, 1000.0f, "%.0f px/s");
                ImGui::SliderFloat("Acceleration", &tracking.accelerationCurve, 1.0f, 5.0f, "%.1f");
                
                ImGui::Spacing();
                ImGui::Text("Status:");
                if (m_MouseTracker->HasTarget()) {
                    auto [x, y] = m_MouseTracker->GetTargetPosition();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), 
                                      "Target Locked: (%.0f, %.0f)", x, y);
                } else {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
                                      "No Target");
                }
                
                m_MouseTracker->SetSettings(tracking);
                
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Logs")) {
                ImGui::Spacing();
                
                if (ImGui::Button("Clear Logs")) {
                    Logger::Instance().Clear();
                }
                
                ImGui::Separator();
                ImGui::BeginChild("LogsScrolling", ImVec2(0, 0), false, 
                                 ImGuiWindowFlags_HorizontalScrollbar);
                
                for (const auto& log : Logger::Instance().GetLogs()) {
                    ImVec4 color;
                    switch (log.level) {
                        case LogLevel::INFO:
                            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                            break;
                        case LogLevel::WARNING:
                            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                            break;
                        case LogLevel::ERROR:
                            color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                            break;
                    }
                    ImGui::TextColored(color, "[%s] %s: %s", 
                                      log.timestamp.c_str(),
                                      log.level == LogLevel::INFO ? "INFO" :
                                      log.level == LogLevel::WARNING ? "WARN" : "ERROR",
                                      log.message.c_str());
                }
                
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void Application::Shutdown() {
    if (!m_Running && m_Window == nullptr) return;
    
    Logger::Instance().Info("Shutting down application...");
    
    // Save settings before shutdown
    if (m_Settings) {
        m_Settings->Save("settings.json");
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    if (m_GLContext) {
        SDL_GL_DeleteContext(m_GLContext);
    }
    
    if (m_Window) {
        SDL_DestroyWindow(m_Window);
    }
    
    SDL_Quit();
    
    Logger::Instance().Info("Application shutdown complete");
}

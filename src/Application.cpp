#include "Application.h"
#include "Config.h"
#include "Logger.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

Application::Application() 
    : window(nullptr), ui(nullptr), tracker(nullptr), splashScreen(nullptr),
      running(false), initialized(false), lastFrameTime(0.0f), deltaTime(0.0f),
      fps(0.0f), frameCount(0), fpsTimer(0.0f) {
}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
    Logger::getInstance().info("Initializing Mouse Tracker Application");
    
    try {
        // Initialize GLFW
        if (!glfwInit()) {
            lastError = "Failed to initialize GLFW";
            Logger::getInstance().critical(lastError);
            return false;
        }
        
        // Create window
        if (!initWindow()) {
            return false;
        }
        
        // Initialize ImGui
        if (!initImGui()) {
            return false;
        }
        
        // Load configuration
        Config& config = Config::getInstance();
        config.load();
        
        // Create components
        tracker = new MouseTracker();
        ui = new UI();
        
        if (config.getGeneralSettings().showSplash) {
            splashScreen = new SplashScreen();
            splashScreen->show();
        }
        
        // Apply config to tracker
        tracker->setFOVRadius(config.getFOVSettings().radius);
        tracker->setStrength(config.getTrackingSettings().strength);
        tracker->setSmoothness(config.getTrackingSettings().smoothness);
        tracker->setEnabled(config.getTrackingSettings().enabled);
        
        initialized = true;
        Logger::getInstance().info("Application initialized successfully");
        return true;
        
    } catch (const std::exception& e) {
        lastError = "Initialization error: " + std::string(e.what());
        Logger::getInstance().critical(lastError);
        return false;
    }
}

bool Application::initWindow() {
    // Set GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    window = glfwCreateWindow(1280, 720, "Mouse Tracker v1.0", nullptr, nullptr);
    if (!window) {
        lastError = "Failed to create GLFW window";
        Logger::getInstance().critical(lastError);
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        lastError = "Failed to initialize GLAD";
        Logger::getInstance().critical(lastError);
        return false;
    }
    
    Logger::getInstance().info("Window created successfully");
    return true;
}

bool Application::initImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    Logger::getInstance().info("ImGui initialized successfully");
    return true;
}

void Application::run() {
    if (!initialized) {
        Logger::getInstance().error("Cannot run - application not initialized");
        return;
    }
    
    running = true;
    Logger::getInstance().info("Application started");
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    while (running && !glfwWindowShouldClose(window)) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - startTime).count() - lastFrameTime;
        lastFrameTime = std::chrono::duration<float>(currentTime - startTime).count();
        
        // Update FPS
        frameCount++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f) {
            fps = static_cast<float>(frameCount) / fpsTimer;
            frameCount = 0;
            fpsTimer = 0.0f;
        }
        
        processInput();
        update(deltaTime);
        render();
    }
    
    Logger::getInstance().info("Application shutting down");
}

void Application::processInput() {
    glfwPollEvents();
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
    }
}

void Application::update(float dt) {
    try {
        // Update mouse tracker
        if (tracker) {
            tracker->update(dt);
        }
        
        // Update splash screen
        if (splashScreen && !splashScreen->isComplete()) {
            splashScreen->update();
        }
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("Update error: " + std::string(e.what()));
    }
}

void Application::render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Render splash screen or main UI
    if (splashScreen && !splashScreen->isComplete()) {
        // Splash screen handles its own rendering
    } else {
        if (ui) {
            ui->setFPS(fps);
            ui->render(*tracker);
            
            if (ui->shouldClose()) {
                running = false;
            }
        }
    }
    
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glfwSwapBuffers(window);
}

void Application::shutdown() {
    if (initialized) {
        // Save configuration
        Config::getInstance().save();
        
        // Cleanup
        delete ui;
        delete tracker;
        delete splashScreen;
        
        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        // Cleanup GLFW
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
        
        initialized = false;
        Logger::getInstance().info("Application shutdown complete");
    }
}

void Application::cleanup() {
    shutdown();
}

#include "Application.h"
#include "Logger.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().info("Starting Mouse Tracker Application");
        
        Application app;
        
        if (!app.initialize()) {
            Logger::getInstance().critical("Failed to initialize application");
            return 1;
        }
        
        app.run();
        app.shutdown();
        
        Logger::getInstance().info("Application exited normally");
        return 0;
        
    } catch (const std::exception& e) {
        Logger::getInstance().critical("Unhandled exception: " + std::string(e.what()));
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        Logger::getInstance().critical("Unknown exception occurred");
        std::cerr << "Fatal error: Unknown exception" << std::endl;
        return 1;
    }
}

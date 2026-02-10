#include "Application.h"
#include "Logger.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Logger::Instance().Info("Starting Mouse Tracker Application...");
        
        Application app;
        
        if (!app.Initialize()) {
            Logger::Instance().Error("Failed to initialize application");
            return 1;
        }
        
        app.Run();
        app.Shutdown();
        
        Logger::Instance().Info("Application exited successfully");
        return 0;
    } catch (const std::exception& e) {
        Logger::Instance().Error(std::string("Fatal error: ") + e.what());
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

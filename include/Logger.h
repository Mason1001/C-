#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
public:
    static Logger& getInstance();
    
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);
    
    void setLogFile(const std::string& filename);
    void setLogLevel(LogLevel level);
    
private:
    Logger();
    ~Logger();
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::string getCurrentTimestamp();
    std::string levelToString(LogLevel level);
    
    std::ofstream logFile;
    LogLevel minLevel;
    std::string logFileName;
};

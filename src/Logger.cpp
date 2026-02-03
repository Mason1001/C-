#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    m_LogFile.open("mousetracker.log", std::ios::out | std::ios::app);
    if (m_LogFile.is_open()) {
        Log(LogLevel::INFO, "=== Logger initialized ===");
    }
}

Logger::~Logger() {
    if (m_LogFile.is_open()) {
        Log(LogLevel::INFO, "=== Logger shutdown ===");
        m_LogFile.close();
    }
}

void Logger::Log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    LogEntry entry;
    entry.level = level;
    entry.message = message;
    entry.timestamp = GetTimestamp();
    
    m_Logs.push_back(entry);
    
    // Write to file
    if (m_LogFile.is_open()) {
        m_LogFile << "[" << entry.timestamp << "] "
                  << LogLevelToString(level) << ": "
                  << message << std::endl;
        m_LogFile.flush();
    }
    
    // Write to console
    std::cout << "[" << entry.timestamp << "] "
              << LogLevelToString(level) << ": "
              << message << std::endl;
}

void Logger::Info(const std::string& message) {
    Log(LogLevel::INFO, message);
}

void Logger::Warning(const std::string& message) {
    Log(LogLevel::WARNING, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::ERROR, message);
}

std::string Logger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::LogLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

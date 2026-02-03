#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <vector>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

struct LogEntry {
    LogLevel level;
    std::string message;
    std::string timestamp;
};

class Logger {
public:
    static Logger& Instance();
    
    void Log(LogLevel level, const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);
    
    const std::vector<LogEntry>& GetLogs() const { return m_Logs; }
    void Clear() { m_Logs.clear(); }
    
private:
    Logger();
    ~Logger();
    
    std::string GetTimestamp() const;
    std::string LogLevelToString(LogLevel level) const;
    
    std::ofstream m_LogFile;
    std::vector<LogEntry> m_Logs;
    std::mutex m_Mutex;
};

#endif // LOGGER_H

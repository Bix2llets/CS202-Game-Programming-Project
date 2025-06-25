#pragma once
#include <string>
#include <iostream>
#include <sstream>

enum class LogLevel {
    TRACE,
    DEBUGG,
    INFO,
    SUCCESS,
    WARNING,
    ERROR,
    CRITICAL,
    EXCEPTION,
    NETWORK,
    PERFORMANCE,
    MEMORY,
    SECURITY
};

class Logger {
private:
    static const char* getColorCode(LogLevel level);
    static const char* getLevelName(LogLevel level);
    static const char* RESET_COLOR;
    
public:
    static void log(LogLevel level, const std::string& message);
    
    static void trace(const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void success(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);
    static void exception(const std::string& message);
    static void network(const std::string& message);
    static void performance(const std::string& message);
    static void memory(const std::string& message);
    static void security(const std::string& message);
    
    template<typename... Args>
    static void logf(LogLevel level, const std::string& format, Args... args) {
        std::ostringstream oss;
        formatMessage(oss, format, args...);
        log(level, oss.str());
    }
    
    static std::string messageAddress(const std::string& message, const void* address) {
        std::ostringstream oss;
        oss << message << " [Address: " << address << "]";
        return oss.str();
    }

private:
    template<typename T>
    static void formatMessage(std::ostringstream& oss, const std::string& format, T&& value) {
        size_t pos = format.find("{}");
        if (pos != std::string::npos) {
            oss << format.substr(0, pos) << value << format.substr(pos + 2);
        } else {
            oss << format;
        }
    }
    
    template<typename T, typename... Args>
    static void formatMessage(std::ostringstream& oss, const std::string& format, T&& value, Args... args) {
        size_t pos = format.find("{}");
        if (pos != std::string::npos) {
            std::string newFormat = format.substr(0, pos) + std::to_string(value) + format.substr(pos + 2);
            formatMessage(oss, newFormat, args...);
        } else {
            oss << format;
        }
    }
};


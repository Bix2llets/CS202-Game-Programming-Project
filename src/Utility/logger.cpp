#include "Utility/logger.hpp"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <iostream>

const char* Logger::RESET_COLOR = "\033[0m";

bool Logger::isLogOn = false;
bool Logger::isTraceOn = true;
bool Logger::isDebugOn = false;
bool Logger::isInfoOn = false;
bool Logger::isSuccessOn = true;
bool Logger::isWarningOn = true;
bool Logger::isErrorOn = true;
bool Logger::isCriticalOn = true;
bool Logger::isExceptionOn = true;
bool Logger::isNetworkOn = true;
bool Logger::isPerformanceOn = true;
bool Logger::isMemoryOn = true;
bool Logger::isSecurityOn = true;

const char* Logger::getColorCode(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:       return "\033[37m";      // White
        case LogLevel::DEBUGG:      return "\033[36m";      // Cyan
        case LogLevel::INFO:        return "\033[34m";      // Blue
        case LogLevel::SUCCESS:     return "\033[32m";      // Green
        case LogLevel::WARNING:     return "\033[33m";      // Yellow
        case LogLevel::ERROR:       return "\033[31m";      // Red
        case LogLevel::CRITICAL:    return "\033[41m";      // Red background
        case LogLevel::EXCEPTION:   return "\033[35m";      // Magenta
        case LogLevel::NETWORK:     return "\033[38;2;255;165;0m";    // Orange (RGB: 255,165,0)
        case LogLevel::PERFORMANCE: return "\033[38;2;255;20;147m";   // Deep Pink (RGB: 255,20,147)
        case LogLevel::MEMORY:      return "\033[38;2;75;0;130m";     // Indigo (RGB: 75,0,130)
        case LogLevel::SECURITY:    return "\033[38;2;220;20;60m";    // Crimson (RGB: 220,20,60)
        default:                    return "\033[0m";       // Reset
    }

}

const char* Logger::getLevelName(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:       return "TRACE";
        case LogLevel::DEBUGG:      return "DEBUG";
        case LogLevel::INFO:        return "INFO";
        case LogLevel::SUCCESS:     return "SUCCESS";
        case LogLevel::WARNING:     return "WARNING";
        case LogLevel::ERROR:       return "ERROR";
        case LogLevel::CRITICAL:    return "CRITICAL";
        case LogLevel::EXCEPTION:   return "EXCEPTION";
        case LogLevel::NETWORK:     return "NETWORK";
        case LogLevel::PERFORMANCE: return "PERFORMANCE";
        case LogLevel::MEMORY:      return "MEMORY";
        case LogLevel::SECURITY:    return "SECURITY";
        default:                    return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::cout << getColorCode(level) 
              << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
              << "[" << getLevelName(level) << "] "
              << message 
              << RESET_COLOR << std::endl;
}

void Logger::trace(const std::string& message) {
    if(!isTraceOn) return;
    log(LogLevel::TRACE, message);
}

void Logger::debug(const std::string& message) {
    if(!isDebugOn) return;
    log(LogLevel::DEBUGG, message);
}

void Logger::info(const std::string& message) {
    if(!isInfoOn) return;
    log(LogLevel::INFO, message);
}

void Logger::success(const std::string& message) {
    if(!isSuccessOn) return;
    log(LogLevel::SUCCESS, message);
}

void Logger::warning(const std::string& message) {
    if(!isWarningOn) return;
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    if(!isErrorOn) return;
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string& message) {
    if(!isCriticalOn) return;
    log(LogLevel::CRITICAL, message);
}

void Logger::exception(const std::string& message) {
    if(!isExceptionOn) return;
    log(LogLevel::EXCEPTION, message);
}

void Logger::network(const std::string& message) {
    if(!isNetworkOn) return;
    log(LogLevel::NETWORK, message);
}

void Logger::performance(const std::string& message) {
    if(!isPerformanceOn) return;
    log(LogLevel::PERFORMANCE, message);
}

void Logger::memory(const std::string& message) {
    if(!isMemoryOn) return;
    log(LogLevel::MEMORY, message);
}

void Logger::security(const std::string& message) {
    if(!isSecurityOn) return;
    log(LogLevel::SECURITY, message);
}

void Logger::setLogState(bool enabled) {
    isLogOn = enabled;
}

void Logger::setTraceState(bool enabled) {
    isTraceOn = enabled;
}

void Logger::setDebugState(bool enabled) {
    isDebugOn = enabled;
}

void Logger::setInfoState(bool enabled) {
    isInfoOn = enabled;
}

void Logger::setSuccessState(bool enabled) {
    isSuccessOn = enabled;
}

void Logger::setWarningState(bool enabled) {
    isWarningOn = enabled;
}

void Logger::setErrorState(bool enabled) {
    isErrorOn = enabled;
}

void Logger::setCriticalState(bool enabled) {
    isCriticalOn = enabled;
}

void Logger::setExceptionState(bool enabled) {
    isExceptionOn = enabled;
}

void Logger::setNetworkState(bool enabled) {
    isNetworkOn = enabled;
}

void Logger::setPerformanceState(bool enabled) {
    isPerformanceOn = enabled;
}

void Logger::setMemoryState(bool enabled) {
    isMemoryOn = enabled;
}

void Logger::setSecurityState(bool enabled) {
    isSecurityOn = enabled;
}

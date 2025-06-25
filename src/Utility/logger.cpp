#include "Utility/logger.hpp"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <iostream>

const char* Logger::RESET_COLOR = "\033[0m";

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
    log(LogLevel::TRACE, message);
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUGG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::success(const std::string& message) {
    log(LogLevel::SUCCESS, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LogLevel::CRITICAL, message);
}

void Logger::exception(const std::string& message) {
    log(LogLevel::EXCEPTION, message);
}

void Logger::network(const std::string& message) {
    log(LogLevel::NETWORK, message);
}

void Logger::performance(const std::string& message) {
    log(LogLevel::PERFORMANCE, message);
}

void Logger::memory(const std::string& message) {
    log(LogLevel::MEMORY, message);
}

void Logger::security(const std::string& message) {
    log(LogLevel::SECURITY, message);
}

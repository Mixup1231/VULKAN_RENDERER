#pragma once

#include "defines.h"

#include <string>
#include <windows.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <fstream>

#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 1

#ifndef MARS_RELEASE
#define LOG_LEVEL_TRACE 1
#define LOG_LEVEL_DEBUG 1
#else
#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 0
#endif

#if LOG_LEVEL_TRACE
    #define MARS_TRACE(format, ...) mars::Logger::Get().LogOutput(mars::LogLevel::LOG_TRACE, format, __VA_ARGS__)
    #define MARS_TRACE_FILE(format, ...) mars::Logger::Get().LogMessage(mars::LogLevel::LOG_TRACE, format, __VA_ARGS__)
#else
    #define MARS_TRACE(format, ...)
    #define MARS_TRACE_FILE(format, ...)
#endif
#if LOG_LEVEL_DEBUG
    #define MARS_DEBUG(format, ...) mars::Logger::Get().LogOutput(mars::LogLevel::LOG_DEBUG, format, __VA_ARGS__)
    #define MARS_DEBUG_FILE(format, ...) mars::Logger::Get().LogMessage(mars::LogLevel::LOG_DEBUG, format, __VA_ARGS__)
#else
    #define MARS_DEBUG(format, ...)
    #define MARS_DEBUG_FILE(format, ...)
#endif
#if LOG_LEVEL_INFO
    #define MARS_INFO(format, ...) mars::Logger::Get().LogOutput(mars::LogLevel::LOG_INFO, format, __VA_ARGS__)
    #define MARS_INFO_FILE(format, ...) mars::Logger::Get().LogMessage(mars::LogLevel::LOG_INFO, format, __VA_ARGS__)
#else
    #define MARS_INFO(format, ...)
    #define MARS_INFO_FILE(format, ...)
#endif
#if LOG_LEVEL_WARN
    #define MARS_WARN(format, ...) mars::Logger::Get().LogOutput(mars::LogLevel::LOG_WARN, format, __VA_ARGS__)
    #define MARS_WARN_FILE(format, ...) mars::Logger::Get().LogMessage(mars::LogLevel::LOG_WARN, format, __VA_ARGS__)
#else
    #define MARS_WARN(format, ...)
    #define MARS_WARN_FILE(format, ...)
#endif

#define MARS_ERROR(format, ...) mars::Logger::Get().LogOutput(mars::LogLevel::LOG_ERROR, format, __VA_ARGS__)
#define MARS_ERROR_FILE(format, ...) mars::Logger::Get().LogMessage(mars::LogLevel::LOG_ERROR, format, __VA_ARGS__)

#define MARS_FATAL(format, ...) mars::Logger::Get().LogOutput(mars::LogLevel::LOG_FATAL, format, __VA_ARGS__)
#define MARS_FATAL_FILE(format, ...) mars::Logger::Get().LogMessage(mars::LogLevel::LOG_FATAL, format, __VA_ARGS__)

namespace mars {
    

enum class LogLevel {
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_INFO,
    LOG_WARN,
    LOG_DEBUG,
    LOG_TRACE       
};

class MARS_API Logger {
public:
    static constexpr u8 CONSOLE_TRACE_ATTRIBUTE    = 15;
    static constexpr u8 CONSOLE_DEBUG_ATTRIBUTE    = 11;
    static constexpr u8 CONSOLE_INFO_ATTRIBUTE     = 10;
    static constexpr u8 CONSOLE_WARN_ATTRIBUTE     = 14;
    static constexpr u8 CONSOLE_ERROR_ATTRIBUTE    = 12;
    static constexpr u8 CONSOLE_FATAL_ATTRIBUTE    = BACKGROUND_RED;

    static constexpr Size DEFAULT_BUFFER_SIZE      = 1024;
    static constexpr Size DEFAULT_FILE_BUFFER_SIZE = 4096;

  public:
    Logger(const std::string& filename, Size bufferSize = DEFAULT_FILE_BUFFER_SIZE);
    ~Logger(void);

    void LogMessage(LogLevel level, const char* format, ...);
    void LogOutput(LogLevel level, const char* format, ...);
    void Flush(void);

    static Logger& Get(void);
    
private:        
    std::ofstream m_LogFile;
    std::vector<std::string> m_LogBuffer;
    Size m_BufferSize;
    std::mutex m_Mutex;

private:
    void LogMessageVariadic(LogLevel level, const char* format, va_list args);
    std::string GetTime(void);
    std::string GetLogLevelPrepend(LogLevel level);
};

} // namespace mars

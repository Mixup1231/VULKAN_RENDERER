#include "logger.h"

#include <iostream>

namespace mars {

Logger::Logger(const std::string& filename, Size bufferSize) :
    m_BufferSize(bufferSize) {
    LogOutput(LogLevel::LOG_INFO, "Initialising logger...\n");
    m_LogFile.open(filename, std::ios::app);
    if (!m_LogFile.is_open()) {
        LogOutput(LogLevel::LOG_WARN, "Failed to open file %s...\n", filename.c_str());
    } else {
        LogOutput(LogLevel::LOG_INFO, "Opened file %s for logging...\n", filename.c_str());        
    }
}

Logger::~Logger() {
    LogOutput(LogLevel::LOG_INFO, "Uninitialising logger, flushing...\n");
    Flush();
    if (m_LogFile.is_open()) {
        LogOutput(LogLevel::LOG_INFO, "Closing logger file...\n");
        m_LogFile.close();    
    }        
}

Logger& Logger::Get(void) {
    static Logger instance("Application.log");    
    return instance;
}

void Logger::LogMessage(LogLevel level, const char* format, ...) {
    std::lock_guard<std::mutex> lock(m_Mutex);

    va_list args;
    va_start(args, format);
    LogMessageVariadic(level, format, args);
    va_end(args);

    if (m_LogBuffer.size() >= m_BufferSize) {
        Flush();    
    }   
}

void Logger::LogMessageVariadic(LogLevel level, const char* format, va_list args) {
    const Size maxBufferSize = DEFAULT_BUFFER_SIZE;
    char buffer[maxBufferSize];
    vsnprintf(buffer, maxBufferSize, format, args);

    std::string logEntry = "[" + GetTime() + "] " + GetLogLevelPrepend(level) + " " + buffer;    
    m_LogBuffer.push_back(logEntry);
}

void Logger::Flush(void) {
    for (const auto& entry : m_LogBuffer) {
        m_LogFile << entry << std::endl;    
    }
    m_LogBuffer.clear();        
}
    

std::string Logger::GetTime(void) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);    
    std::tm buffer;
    localtime_s(&buffer, &in_time_t);

    std::ostringstream ss;
    ss << std::put_time(&buffer, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::GetLogLevelPrepend(LogLevel level) {
    switch (static_cast<u8>(level)) {
    case static_cast<u8>(LogLevel::LOG_TRACE):
        return "[TRACE]";
    case static_cast<u8>(LogLevel::LOG_DEBUG):
        return "[DEBUG]";
    case static_cast<u8>(LogLevel::LOG_INFO):
        return "[INFO]";
    case static_cast<u8>(LogLevel::LOG_WARN):
        return "[WARN]";
    case static_cast<u8>(LogLevel::LOG_ERROR):
        return "[ERROR]";
    case static_cast<u8>(LogLevel::LOG_FATAL):
        return "[FATAL]";
    }

    return "";
}

void Logger::LogOutput(LogLevel level, const char* format, ...) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
        
    if (GetConsoleScreenBufferInfo(consoleHandle, &csbi)) {
                
        char buffer[DEFAULT_BUFFER_SIZE];
        char message[DEFAULT_BUFFER_SIZE + 50];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, DEFAULT_BUFFER_SIZE, format, args);
        va_end(args);

        switch (static_cast<u8>(level)) {
            case static_cast<u8>(LogLevel::LOG_TRACE):
                SetConsoleTextAttribute(consoleHandle, CONSOLE_TRACE_ATTRIBUTE);
                break;
            case static_cast<u8>(LogLevel::LOG_DEBUG):
                SetConsoleTextAttribute(consoleHandle, CONSOLE_DEBUG_ATTRIBUTE);
                break;
            case static_cast<u8>(LogLevel::LOG_INFO):
                SetConsoleTextAttribute(consoleHandle, CONSOLE_INFO_ATTRIBUTE);
                break;
            case static_cast<u8>(LogLevel::LOG_WARN):
                SetConsoleTextAttribute(consoleHandle, CONSOLE_WARN_ATTRIBUTE);
                break;
            case static_cast<u8>(LogLevel::LOG_ERROR):
                SetConsoleTextAttribute(consoleHandle, CONSOLE_ERROR_ATTRIBUTE);
                break;
            case static_cast<u8>(LogLevel::LOG_FATAL):
                SetConsoleTextAttribute(consoleHandle, CONSOLE_FATAL_ATTRIBUTE);
                break;
        }

        snprintf(message, sizeof(message), "%s   %s", GetLogLevelPrepend(level).c_str(), buffer);
        printf(message);
        SetConsoleTextAttribute(consoleHandle, csbi.wAttributes);
    }
}

} // namespace mars

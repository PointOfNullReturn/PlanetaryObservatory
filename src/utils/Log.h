#ifndef PLANETARYOBSERVATORY_UTILS_LOG_H
#define PLANETARYOBSERVATORY_UTILS_LOG_H

#include <string_view>

enum class LogLevel
{
    Info,
    Warning,
    Error,
    Debug
};

namespace Log
{

void logMessage(LogLevel level, std::string_view message);

inline void info(std::string_view message)
{
    logMessage(LogLevel::Info, message);
}

inline void warn(std::string_view message)
{
    logMessage(LogLevel::Warning, message);
}

inline void error(std::string_view message)
{
    logMessage(LogLevel::Error, message);
}

#if defined(PO_ENABLE_DEBUG_LOGS)
inline constexpr bool kDebugLoggingEnabled = true;
#else
inline constexpr bool kDebugLoggingEnabled = false;
#endif

inline void debug(std::string_view message)
{
    if (kDebugLoggingEnabled)
    {
        logMessage(LogLevel::Debug, message);
    }
}

} // namespace Log

#endif // PLANETARYOBSERVATORY_UTILS_LOG_H

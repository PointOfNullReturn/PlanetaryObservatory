#include "utils/Log.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace
{
std::string_view levelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARN";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Debug:
            return "DEBUG";
    }
    return "UNKNOWN";
}

std::ostream& streamForLevel(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Warning:
        case LogLevel::Error:
            return std::cerr;
        default:
            return std::cout;
    }
}

std::string timestamp()
{
    using clock = std::chrono::system_clock;
    const auto now = clock::now();
    const auto timeT = clock::to_time_t(now);
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()) % 1000000;

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &timeT);
#else
    localtime_r(&timeT, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S") << '.' << std::setw(6) << std::setfill('0')
        << micros.count();
    return oss.str();
}

} // namespace

void Log::logMessage(LogLevel level, std::string_view message)
{
    auto& stream = streamForLevel(level);
    stream << '[' << timestamp() << "] [" << levelToString(level) << "] " << message
           << std::endl;
}

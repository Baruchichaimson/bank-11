#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <utility>

#include "logger.hpp"

namespace ilrd
{

static std::string GetTimestamp();
static const char* LogLevelToString(Logger::LogLevel level);


/* ---------------- Logger implementation ---------------- */

Logger::Logger()
: m_is_alive(true), m_curr_level(DEBUG)
{
    m_file.open(m_log_path, std::ios::app);
    if (!m_file.is_open())
    {
        std::cerr << "Error opening log file." << std::endl;
    }

    m_writer = std::thread([this]() { this->WriteToFile(); });
}

Logger::~Logger()
{
    m_is_alive.store(false, std::memory_order_release);

    
    LogArgs sentinel;
    sentinel.msg = std::string();      
    sentinel.level = Logger::DEBUG;
    sentinel.file_name = std::string("The logger is close!!");
    sentinel.line = 0;

    m_queue.push(std::move(sentinel));

    if (m_writer.joinable())
    {
        m_writer.join();
    }

    if (m_file.is_open())
    {
        m_file.close();
    }
}

void Logger::SetLevel(LogLevel level) noexcept
{
    m_curr_level = level;
}

void Logger::Log(const std::string& msg, LogLevel level, std::string file_name, int line)
{
    if (level > m_curr_level)
    {
        return; 
    }

    LogArgs args;
    args.msg = msg;
    args.level = level;
    args.file_name = std::move(file_name);
    args.line = line;

    m_queue.push(std::move(args));
}

ssize_t Logger::WriteToFile()
{
    while (m_is_alive.load(std::memory_order_acquire) || !m_queue.empty())
    {
        LogArgs args;
        m_queue.pop(&args); 

        std::string ts = GetTimestamp();
        m_file << ts << " [" << LogLevelToString(args.level) << "] "
               << args.file_name << ":" << args.line << " " << args.msg << '\n';
        m_file.flush();
    }

    return 0;
}

static const char* LogLevelToString(Logger::LogLevel level)
{
    switch (level)
    {
        case Logger::ERROR:
           return "ERROR";

        case Logger::WARNING:
           return "WARNING";

        case Logger::DEBUG:
           return "DEBUG";

        case Logger::INFO:
           return "INFO";
           
        default:
           return "UNKNOWN";
    }
}

static std::string GetTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm_snapshot;

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_snapshot, &t);
#else
    localtime_r(&t, &tm_snapshot);
#endif

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << tm_snapshot.tm_mday
        << std::setw(2) << (tm_snapshot.tm_mon + 1)
        << std::setw(4) << (tm_snapshot.tm_year + 1900)
        << ':'
        << std::setw(2) << tm_snapshot.tm_hour
        << ':'
        << std::setw(2) << tm_snapshot.tm_min
        << ':'
        << std::setw(2) << tm_snapshot.tm_sec;

    return oss.str();
}

} // namespace ilrd

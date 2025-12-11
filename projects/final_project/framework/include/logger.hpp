#ifndef __ILRD_LOGGER__ 
#define __ILRD_LOGGER__ 

#include <fstream>      // std::ofstream
#include <sys/types.h>  // ssize_t
#include <thread> // std::thread

#include "waitable_queue.hpp"
#include "handleton.hpp"

namespace ilrd
{
class Logger
{
    
public:
    enum LogLevel
    {
        ERROR, 
        WARNING,
        DEBUG,
        INFO
    };
    
    void SetLevel(LogLevel level = DEBUG) noexcept;
    void Log(const std::string& msg, LogLevel level = DEBUG, std::string file_name = __FILE__, int line = __LINE__);
    
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    
private:
    struct LogArgs
    {
        std::string msg;
        LogLevel level;
        std::string file_name;
        int line;
    };

    friend class Handleton<Logger>;
    friend class Singleton<Logger>;
    Logger();
    ~Logger();
    ssize_t WriteToFile(); 
    
    std::atomic<bool> m_is_alive;
    WaitableQueue<LogArgs> m_queue;
    LogLevel m_curr_level;
    std::ofstream m_file;
    std::thread m_writer;
}; // class Logger

#define LOG(msg, level) (logger.Log(std::string(msg), static_cast<enum ilrd::Logger::LogLevel>((level)),__FILE__,__LINE__ ))

static const std::string m_log_path = "./log_file.txt";
} // ilrd

#endif // __ILRD_LOGGER__

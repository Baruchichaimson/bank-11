/**************************************
Exercise: 	final project - dir monitor
Date:		14/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#ifndef __ILRD_DIR_MONITOR_HPP__
#define __ILRD_DIR_MONITOR_HPP__

#include <thread> //std::thread
#include <atomic> // std::atomic
#include <sys/inotify.h> // inotify_init


#include "dispatcher.hpp"

namespace ilrd
{

const std::size_t BUFFER_SIZE = 1024 * (sizeof(inotify_event) + 16);

class DirMonitor
{
public:
    explicit DirMonitor(const std::string& dir);
    ~DirMonitor() noexcept;

    void Run(); // Starts the monitoring thread

    void Register(BaseCallback<const std::string&>* cb);
    void Unregister(BaseCallback<const std::string&>* cb);

private:
    void ListeningLoop();
    Dispatcher<const std::string&> m_dispatcher;
    std::thread m_listener;
    std::atomic<bool> m_is_running;
    char m_buffer[BUFFER_SIZE];
    std::string m_dir_path;
    int m_inotify_fd;
    int m_watch_descriptor;
};

} // namespace ilrd

#endif // __ILRD_DIR_MONITOR_HPP__

#ifndef __ILRD_DIR_MONITOR_HPP__
#define __ILRD_DIR_MONITOR_HPP__

#include <string>
#include <thread>
#include <atomic>

#include "dispatcher.hpp"

namespace ilrd
{

class DirMonitor
{
public:
    explicit DirMonitor(const std::string& dir);
    ~DirMonitor() noexcept;

    void Run(); // Starts the monitoring thread
    void Stop();

    void Register(BaseCallback<const std::string&>* cb);
    void Unregister(BaseCallback<const std::string&>* cb);

private:
    static void ListeningLoop(DirMonitor* monitor);

    Dispatcher<const std::string&> m_dispatcher;
    std::thread m_listener;
    std::atomic<bool> m_is_running;

    std::string m_dir_path;
    int m_inotify_fd;
    int m_watch_descriptor;
};

} // namespace ilrd

#endif // __ILRD_DIR_MONITOR_HPP__

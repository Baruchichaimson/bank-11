#include <sys/inotify.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

#include "dirmonitor.hpp"

namespace ilrd
{

static constexpr size_t EVENT_BUF_LEN = 1024 * (sizeof(inotify_event) + 16);

DirMonitor::DirMonitor(const std::string& dir)
: m_dir_path(dir), m_inotify_fd(-1), m_watch_descriptor(-1), m_is_running(false)
{
    m_inotify_fd = inotify_init1(IN_NONBLOCK);
    if (m_inotify_fd < 0)
    {
        throw std::runtime_error("Failed to initialize inotify");
    }

    m_watch_descriptor = inotify_add_watch(m_inotify_fd, m_dir_path.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY);

    if (m_watch_descriptor < 0)
    {
        close(m_inotify_fd);
        throw std::runtime_error("Failed to add watch on directory");
    }
}

DirMonitor::~DirMonitor() noexcept
{
    Stop();

    if (m_watch_descriptor >= 0)
    {
        inotify_rm_watch(m_inotify_fd, m_watch_descriptor);
    }

    close(m_inotify_fd);

    if (m_listener.joinable())
    {
        m_listener.join();
    }
}

void DirMonitor::Run()
{
    m_is_running.store(true, std::memory_order_release);
    m_listener = std::thread(ListeningLoop, this);
}

void DirMonitor::Stop()
{
    bool was_running = m_is_running.exchange(false, std::memory_order_acq_rel);

    if (was_running && m_listener.joinable())
    {
        m_listener.join();
    }
}

void DirMonitor::Register(BaseCallback<const std::string&>* cb)
{
    m_dispatcher.Subscribe(cb);
}

void DirMonitor::Unregister(BaseCallback<const std::string&>* cb)
{
    m_dispatcher.Unsubscribe(cb);
}

void DirMonitor::ListeningLoop(DirMonitor* monitor)
{
    char buffer[EVENT_BUF_LEN];

    while (monitor->m_is_running)
    {
        int length = read(monitor->m_inotify_fd, buffer, EVENT_BUF_LEN);

        if (length < 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue; // Non-blocking read – ignore empty reads
        }

        int i = 0;

        while (i < length)
        {
            inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);

            if (event->len > 0)
            {
                std::string filename(event->name);

                // FILTER ONLY .so FILES
                if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".so")
                {
                    std::string fullpath = monitor->m_dir_path + "/" + filename;

                    if (event->mask & IN_CREATE)
                    {
                        monitor->m_dispatcher.Broadcast(std::string("ADD:") + fullpath);
                    }
                    else if (event->mask & IN_MODIFY)
                    {
                        monitor->m_dispatcher.Broadcast(std::string("CHANGE:") + fullpath);
                    }
                    else if (event->mask & IN_DELETE)
                    {
                        monitor->m_dispatcher.Broadcast(std::string("REMOVE:") + fullpath);
                    }
                }
            }

            i += sizeof(inotify_event) + event->len;
        }
    }
}

} // namespace ilrd

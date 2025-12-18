/**************************************
Exercise: 	final project - dir monitor
Date:		14/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#include <stdexcept> // std::runtime_error
#include <cerrno> // errno
#include <unistd.h> // close

#include "dirmonitor.hpp"
#include "logger.hpp"

namespace ilrd
{

DirMonitor::DirMonitor(const std::string& dir) : m_dir_path(std::move(dir)), m_dispatcher(Dispatcher<const std::string&>()),
        m_buffer{0}, m_inotify_fd(-1), m_watch_descriptor(-1), m_is_running(false)
{
    LOG_DEBUG("DirMonitor ctor entered");

    m_inotify_fd = inotify_init1(IN_NONBLOCK);
    if (m_inotify_fd < 0)
    {
        throw std::runtime_error("Failed to initialize inotify");
    }

    m_watch_descriptor = inotify_add_watch(m_inotify_fd, m_dir_path.c_str(),
                                           IN_CREATE | IN_CLOSE_WRITE | IN_MOVED_TO);

    if (m_watch_descriptor < 0)
    {
        close(m_inotify_fd);
        throw std::runtime_error("Failed to add watch on directory");
    }

    LOG_DEBUG("DirMonitor ctor exit");
}

DirMonitor::~DirMonitor() noexcept
{
    LOG_DEBUG("DirMonitor dtor entered");

    m_is_running.store(false);

    if (m_watch_descriptor >= 0 && m_listener.joinable())
    {
        m_listener.join();
        inotify_rm_watch(m_inotify_fd, m_watch_descriptor);
        close(m_inotify_fd);
    }

    LOG_DEBUG("DirMonitor dtor exit");
}

void DirMonitor::Run()
{
    LOG_DEBUG("Run entered");

    m_is_running.store(true);
    m_listener = std::thread(&DirMonitor::ListeningLoop, this);

    LOG_DEBUG("Run exit");
}

void DirMonitor::Register(BaseCallback<const std::string&>* cb)
{
    m_dispatcher.Subscribe(cb);
}

void DirMonitor::Unregister(BaseCallback<const std::string&>* cb)
{
    m_dispatcher.Unsubscribe(cb);
}

void DirMonitor::ListeningLoop()
{
    LOG_DEBUG("ListeningLoop entered");

    while (m_is_running.load())
    {
        ssize_t length = read(m_inotify_fd, m_buffer, sizeof(m_buffer));

        if (length < 0)
        {
            if (EAGAIN == errno || EWOULDBLOCK == errno)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }
            else
            {
                throw std::runtime_error("read error");
            }
 
        }

        std::size_t i = 0;

        while (i < length)
        {
            inotify_event* event = reinterpret_cast<inotify_event*>(&m_buffer[i]);

            if (event->len > 0)
            {
                std::string filename(event->name);

                if (filename.size() > 0 && (event->mask & (IN_CREATE | IN_CLOSE_WRITE | IN_MOVED_TO)))
                {
                    std::string fullpath = m_dir_path + "/" + filename;

                    m_dispatcher.Broadcast(fullpath);
                }
            }

            i += sizeof(inotify_event) + event->len;
        }
    }
    LOG_DEBUG("ListeningLoop exit");
}

} // namespace ilrd

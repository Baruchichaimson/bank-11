/**************************************
Exercise: 	final project - reactor
Date:		16/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/
#include <stdexcept> // std::runtime_error

#include "reactor.hpp"

namespace ilrd
{

Reactor::Reactor(const std::shared_ptr<IListener> listener)
    : m_listener(listener)
    , m_is_running(false)
{
}

void Reactor::Add(int fd, Mode mode, std::function<void(int, Mode)> callback)
{
    fd_pair key(fd, mode);
    m_callbacks[key] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
    fd_pair key(fd, mode);
    if (m_callbacks.find(key) != m_callbacks.end())
    {
        m_callbacks.erase(key);
    }
}

void Reactor::Run()
{
    if (m_is_running)
    {
        throw std::runtime_error("reactor is already running");
    }

    m_is_running = true;

    while (m_is_running && !m_callbacks.empty())
    {
        std::vector<fd_pair> fds;
        fds.reserve(m_callbacks.size());

        for (const auto& entry : m_callbacks)
        {
            fds.push_back(entry.first);
        }

        if (fds.empty())
        {
            break;
        }

        fds = m_listener->Listen(fds);

        for (const auto& fd : fds)
        {
            if (!m_is_running)
            {
                break;
            }

            auto it = m_callbacks.find(fd);

            if (it != m_callbacks.end())
            {
                it->second(fd.first, fd.second);
            }
        }
    }
}

void Reactor::Stop()
{
    m_is_running = false;
}

} // namespace ilrd

#include <atomic>
#include <thread>
#include <chrono>
#include <memory>

#include "reactor.hpp"

namespace ilrd
{

Reactor::Reactor(const std::shared_ptr<IListener> listener)
    : m_listener(listener)
    , m_is_running(false)
{
}

void Reactor::Add(int fd, std::function<void(int, Mode)> callback)
{
    fd_pair key(fd, READ);
    m_callbacks[key] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
    fd_pair key(fd, mode);
    m_callbacks.erase(key);
}

void Reactor::Run()
{
    m_is_running.store(true);

    while (m_is_running.load())
    {
        std::vector<fd_pair> fds;
        fds.reserve(m_callbacks.size());

        for (const auto& entry : m_callbacks)
        {
            fds.push_back(entry.first);
        }

        if (fds.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        /* Blocking listen */
        std::vector<fd_pair> ready =
            m_listener->Listen(fds);

        for (const auto& fd : ready)
        {
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
    m_is_running.store(false);
}

} // namespace ilrd

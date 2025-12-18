/**************************************
Exercise: 	final project - reactor
Date:		16/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#include <stdexcept> // std::runtime_error
#include <algorithm> // std::max

#include "select_listener.hpp"

namespace ilrd
{

std::vector<Reactor::fd_pair> SelectListener::Listen(const std::vector<Reactor::fd_pair>& set)
{
    fd_set readfds;
    fd_set writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);

    int max_fd = -1;
    for (const auto& pair : set)
    {
        if (pair.second == Reactor::READ)
        {
            FD_SET(pair.first, &readfds);
        }
        else if (pair.second == Reactor::WRITE)
        {
            FD_SET(pair.first, &writefds);
        }

        max_fd = std::max(max_fd, pair.first);
    }

    int rc = select(max_fd + 1, &readfds, &writefds, nullptr, nullptr);
    if (rc < 0)
    {
        throw std::runtime_error("select failed");
    }

    std::vector<Reactor::fd_pair> ready;
    ready.reserve(set.size());

    for (const auto& pair : set)
    {
        bool is_ready = (pair.second == Reactor::READ && FD_ISSET(pair.first, &readfds)) || (pair.second == Reactor::WRITE && FD_ISSET(pair.first, &writefds));

        if (is_ready)
        {
            ready.push_back(pair);
        }
    }

    return ready;
}

} // namespace ilrd

#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include <memory>
#include <unistd.h>

#include "reactor.hpp"

using namespace ilrd;

class StdinListener : public Reactor::IListener
{
public:
    std::vector<Reactor::fd_pair>
    Listen(const std::vector<Reactor::fd_pair>& set) override
    {
        fd_set readfds;
        FD_ZERO(&readfds);

        int maxfd = -1;
        for (const auto& p : set)
        {
            if (p.second == Reactor::READ)
            {
                FD_SET(p.first, &readfds);
                maxfd = std::max(maxfd, p.first);
            }
        }

        select(maxfd + 1, &readfds, nullptr, nullptr, nullptr);

        std::vector<Reactor::fd_pair> ready;
        for (const auto& p : set)
        {
            if (p.second == Reactor::READ && FD_ISSET(p.first, &readfds))
            {
                ready.push_back(p);
            }
        }

        return ready;
    }
};

/* -------------------------------------------------------
   Callback
------------------------------------------------------- */
static int g_counter = 0;

void StdinCallback(int fd, Reactor::Mode)
{
    char buf[128] = {0};
    ssize_t n = read(fd, buf, sizeof(buf));

    if (n > 0)
    {
        std::cout << "[STDIN CALLBACK] " << buf << std::endl;
        ++g_counter;
    }
}

/* -------------------------------------------------------
   MAIN
------------------------------------------------------- */
int main()
{
    std::cout << "==== STDIN Reactor Test ====\n";
    std::cout << "Type something and press ENTER\n";

    auto listener = std::make_shared<StdinListener>();
    Reactor reactor(listener);

    reactor.Add(STDIN_FILENO, StdinCallback);

    std::thread t([&reactor]()
    {
        reactor.Run();
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));

    reactor.Stop();
    t.join();

    std::cout << "STDIN events count = " << g_counter << std::endl;

    assert(g_counter > 0 && "No STDIN events received!");

    std::cout << "==== STDIN Test PASSED ====\n";
    return 0;
}

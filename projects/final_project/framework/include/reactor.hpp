#ifndef __ILRD_REACTOR__
#define __ILRD_REACTOR__

#include <vector> // std::vector
#include <unordered_map>
#include <functional>

namespace ilrd
{
 
class Reactor
{
public:
    enum Mode
    {
        READ,
        WRITE
    };

    using fd_pair = std::pair<int, Reactor::Mode>;
    
    class IListener
    {
    public:
        virtual ~IListener() = default;
        virtual std::vector<fd_pair> Listen(const std::vector<fd_pair>& set) = 0;
    };
    
    explicit Reactor(const std::shared_ptr<IListener> listener);
    ~Reactor() noexcept = default;
    
    Reactor(const Reactor& other) = delete;
    Reactor& operator=(const Reactor& other) = delete;
    Reactor(const Reactor&& other) = delete;
    Reactor& operator=(const Reactor&& other) = delete;
    
    void Add (int fd, std::function<void(int, Mode)> callback);
    void Remove (int fd, Mode mode);                  
    void Run ();
    void Stop ();
    
private:
    class Hasher
    {
        std::size_t operator()(const fd_pair& pair) const noexcept
        {
            std::size_t h1 = std::hash<std::string>{}(pair.first_name);
            std::size_t h2 = std::hash<std::string>{}(pair.second);
            return h1 ^ (h2 << 1);
        }
    };
    
    std::unordered_map<fd_pair, std::function<void(int, Mode)>, Hasher> m_callbacks;
    const std::shared_ptr<IListener> m_listener;
    std::atomic<bool> m_is_running;    
    
}; // class Reactor
} // ilrd

#endif //__ILRD_REACTOR__
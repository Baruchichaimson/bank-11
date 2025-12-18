/**************************************
Exercise: 	final project - reactor
Date:		16/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/
#ifndef __ILRD_REACTOR__
#define __ILRD_REACTOR__

#include <vector> // std::vector
#include <unordered_map> // std::unordered_map
#include <memory> // std::shared_ptr
#include <functional> // std::function
#include <string> // std::hash


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
    
    void Add (int fd, Mode mode, std::function<void(int, Mode)> callback);
    void Remove (int fd, Mode mode);                  
    void Run ();
    void Stop ();
    
private:
    class Hasher
    {
        public:

        std::size_t operator()(const fd_pair& pair) const noexcept
        {
            std::size_t h1 = std::hash<int>{}(pair.first);
            std::size_t h2 = std::hash<Reactor::Mode>{}(pair.second);
            return h1 ^ (h2 << 1);
        }
    };
    
    std::unordered_map<fd_pair, std::function<void(int, Mode)>, Hasher> m_callbacks;
    const std::shared_ptr<IListener> m_listener;
    bool m_is_running;   
    
}; // class Reactor
} // ilrd

#endif //__ILRD_REACTOR__
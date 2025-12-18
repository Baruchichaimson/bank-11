
#ifndef __ILRD_ASYNC_INJECTION__
#define __ILRD_ASYNC_INJECTION__

#include <functional>          // std::function
#include <chrono>              // std::chrono::milliseconds
#include <memory>              // std::shared_ptr

#include "scheduler.hpp"

namespace ilrd
{
class AsyncInjection
{
public:
    explicit AsyncInjection(std::function<bool(void)> action_func,
                            std::chrono::milliseconds );
    
    AsyncInjection(const AsyncInjection& other) = delete;
    AsyncInjection& operator=(const AsyncInjection& other) = delete;

private:

    class AsyncTask : public Scheduler::ISTask
    {
    public:
        explicit AsyncTask(AsyncInjection& father);
        
        void Execute();

    private:
        AsyncInjection& m_father;
    };
    
    void PreformAct();
    void Add();

    ~AsyncInjection() = default;
    
    std::function<bool(void)> m_action_func;
    std::chrono::milliseconds m_interval;
    std::shared_ptr<Scheduler::ISTask> m_task;
    
}; // class AsyncInjection

} // ilrd

#endif  // __ILRD_ASYNC_INJECTION__
#ifndef __ILRD_SCHEDULER__
#define __ILRD_SCHEDULER__

#include <sys/types.h>         // timer_t
#include <memory>              // shared_ptr
#include <time.h>             // timer_create

#include "handleton.hpp"       // Handleton
#include "waitable_queue.hpp"  // Watitable_Queue
#include "pq.hpp"              // Priority_Queue

namespace ilrd
{
class Scheduler
{
public:
    class ISTask // interface 
    {
    public:
        virtual ~ISTask() = default;
        virtual void Execute() = 0;    
    };

    using Duration  = std::chrono::milliseconds;
    using TimePoint = std::chrono::steady_clock::time_point;

    void Add(std::shared_ptr<ISTask> task, const Duration delay);
    
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator=(const Scheduler& other) = delete; 
    Scheduler(Scheduler&& other) = delete;
    Scheduler&& operator=(Scheduler&& other) = delete;    

private:

    using task_t = std::pair<std::shared_ptr<ISTask>, TimePoint>;
    
    friend class Handleton<Scheduler>;
    friend class Singleton<Scheduler>; 

    explicit Scheduler();
    timer_t CreateTimer();
    static void TimerCallback(union sigval sv);
    void SetTimer(TimePoint exec_time);

    ~Scheduler() noexcept(false);   
    
    struct TaskComparator 
    {
        bool operator()(const task_t& a, const task_t& b) const
        {
            return a.second > b.second; 
        }
    };   
                                                        
    WaitableQueue<task_t, PQ<task_t, std::vector<task_t>, TaskComparator>> m_queue;
    std::mutex m_run_mutex;
    timer_t m_timer;

}; // class Scheduler
}; // ilrd

#endif //__ILRD_SCHEDULER__


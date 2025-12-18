/*********************************************
 * Exercise:    final project - Scheduler
 * Date:        17/12/2025
 * Developer:   baruchi haimson
 * Reviewer:    
 * Status:      
 **********************************************/

#include <signal.h>    // sigevent
#include <cstring>     // memset
#include <iostream>    // cout
#include <chrono>

#include "scheduler.hpp"

namespace ilrd
{ 

timer_t Scheduler::CreateTimer()
{
    struct sigevent sev;
    std::memset(&sev, 0, sizeof(sev));

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = TimerCallback;
    sev.sigev_value.sival_ptr = this;

    timer_t timer = 0;
    if (timer_create(CLOCK_MONOTONIC, &sev, &timer) == -1)
    {
        throw std::runtime_error("timer_create failed");
    }

    return timer;
}
/* ----------------------------------------- */

Scheduler::Scheduler()
    : m_timer(CreateTimer())
{
}

Scheduler::~Scheduler() noexcept(false) 
{
    if (timer_delete(m_timer) == -1)
    {
        throw std::runtime_error("timer_delete failed");
    }
}

void Scheduler::Add(std::shared_ptr<ISTask> task, const std::chrono::milliseconds delay)
{
    auto exec_time = std::chrono::steady_clock::now() + delay;
    m_queue.push({task, exec_time});
    
    SetTimer(exec_time);
}

void Scheduler::SetTimer(TimePoint exec_time)
{
    auto now = std::chrono::steady_clock::now();
    auto delay = exec_time - now;
    
    if (delay.count() < 0)
    {
        delay = std::chrono::nanoseconds(1);
    }
    
    auto delay_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(delay);
    
    struct itimerspec its;
    std::memset(&its, 0, sizeof(its));
    
    its.it_value.tv_sec = delay_ns.count() / 1000000000;
    its.it_value.tv_nsec = delay_ns.count() % 1000000000;
    
    if (timer_settime(m_timer, 0, &its, nullptr) == -1)
    {
        throw std::runtime_error("timer_settime failed");
    }

}

void Scheduler::TimerCallback(union sigval sv)
{
    std::cout << "TimerCallback: started" << std::endl;
    
    Scheduler* sched = static_cast<Scheduler*>(sv.sival_ptr);
    
    std::cout << "TimerCallback: about to pop..." << std::endl;
    
    task_t t;
    sched->m_queue.pop(&t);
    

    std::cout << "TimerCallback: popped! Executing..." << std::endl;

    auto now = std::chrono::steady_clock::now();
    
    if (t.second <= now)
    {
        t.first->Execute();
    }
    else
    {
        sched->m_queue.push(t);
        sched->SetTimer(t.second);
        return;
    }
    
    task_t next;
    if (sched->m_queue.pop(&next, std::chrono::milliseconds(0)))
    {
        const auto nextTime = next.second;
        sched->m_queue.push(next);
        sched->SetTimer(nextTime);
    }
}

} // namespace ilrd

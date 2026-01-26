#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <memory>              // shared_ptr
#include <mutex>               // timed_mutex
#include <queue>               // queue
#include <thread>              // thread
#include <unordered_map>       // unordered_map
#include <atomic>              // atomic
#include <condition_variable>  // condition_variable_any
#include <semaphore>           // counting_semaphore
#include <utility>             // pair

#include "pq.hpp"              // PriorityQueue
#include "waitable_queue.hpp"  // WaitableQueue

namespace ilrd
{
class ThreadPool
{
public:
/**********************************
              ITPTask
**********************************/
    class ITPTask // interface 
    {
    public:
        virtual ~ITPTask() = default;
        virtual void Execute() = 0;        
    };

    enum Priority
    {
        KILL = 0,
        PAUSE = 1,
        HIGH = 2,
        MEDIUM = 3,
        LOW = 4
    };

    using task = std::pair<std::shared_ptr<ITPTask>, Priority>;
    struct Comp
    {
        bool operator()(const task& a, const task& b) const
        {
            return a.second > b.second;
        }
    };

private:
    class WorkerThread;

public:
    ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency()); // can throw std::bad_alloc
    ~ThreadPool() noexcept;
    
    void Add(task new_task) noexcept;
    void Run() noexcept;
    void Pause() noexcept;
    void Stop() noexcept;
    void SetNumOfThreads(std::size_t new_num_threads); // can throw std::bad_alloc
    
    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool&& other) = delete;
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(const ThreadPool&& other) = delete;     

private:
    WaitableQueue<task, PQ<task, std::vector<task>, Comp>> m_tasks;
    WaitableQueue<WorkerThread*> m_dead_pool;
    
    std::atomic<bool> m_is_running; 
    std::counting_semaphore<> m_run_sem;
    std::size_t m_num_threads;  

    class WorkerThread
    {
    public:
        inline static thread_local bool m_is_alive = true;

        WorkerThread(ThreadPool& thread_pool) : m_thread_pool(thread_pool), m_thread(&WorkerThread::RunThread, this)
        {
        }

        ~WorkerThread() noexcept
        {
            m_is_alive = false;
            if(m_thread.joinable()) m_thread.join();
        }
        
        class BadAppleKill : public ITPTask
        {
        public:
            virtual void Execute()
            {
                WorkerThread::m_is_alive = false;
            } 
        }; // class BadAppleKill

        class BadApplePause : public ITPTask
        {
        public:
        friend ThreadPool;

            BadApplePause(std::counting_semaphore<>& sem) : m_sem(sem){}
            virtual void Execute()
            {
                m_sem.acquire();
            } 

        private:
            std::counting_semaphore<>& m_sem;
        }; // class BadApplePause

    private:
        void RunThread()
        {
            while(m_is_alive)
            {
                m_thread_pool.m_run_sem.acquire();
                task new_task;
                m_thread_pool.m_tasks.pop(&new_task);
                new_task.first->Execute();
                if (m_thread_pool.m_is_running.load()) m_thread_pool.m_run_sem.release();
            }

            m_thread_pool.m_dead_pool.push(this);
        }

        ThreadPool& m_thread_pool;
        std::thread m_thread;

    }; // class WorkerThread
}; // class ThreaPool
} // ilrd

#endif //__ILRD_THREAD_POOL__

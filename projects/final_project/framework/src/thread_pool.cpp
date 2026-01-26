#include "thread_pool.hpp"
#include "logger.hpp"

namespace ilrd
{

/**********************************
            ThreadPool
**********************************/

ThreadPool::ThreadPool(std::size_t num_threads) : m_is_running(false), m_run_sem(num_threads), m_num_threads(0)
{
    LOG_INFO("ThreadPool Ctor");

    SetNumOfThreads(num_threads);
}

ThreadPool::~ThreadPool() noexcept
{
    LOG_INFO("ThreadPool Dtor");

    ThreadPool::Stop();
}

void ThreadPool::Add(task new_task) noexcept
{
    LOG_INFO("ThreadPool Add");

    m_tasks.push(new_task);
}

void ThreadPool::Run() noexcept
{
    LOG_INFO("ThreadPool Run");

    if(m_is_running.load()) return; // already running 

    m_is_running.store(true);
    m_run_sem.release(m_num_threads);
}

void ThreadPool::Pause() noexcept
{
    LOG_INFO("ThreadPool Pause");

    if(!m_is_running.load()) return; // already paused
    m_is_running.store(false);

    for (std::size_t i = 0; i < m_num_threads; ++i)
    {
        m_tasks.push({std::make_shared<WorkerThread::BadApplePause>(m_run_sem), PAUSE});
    }
}

void ThreadPool::Stop() noexcept
{
    LOG_INFO("ThreadPool Stop");

    ThreadPool::SetNumOfThreads(std::size_t(0));
}

void ThreadPool::SetNumOfThreads(std::size_t new_num_threads) 
{
    LOG_INFO("ThreadPool SetNumOfThreads");
    
    // Shrink
    while (m_num_threads > new_num_threads)
    {
        m_tasks.push({std::make_shared<WorkerThread::BadAppleKill>(), KILL});
        if (!m_is_running.load()) 
        {
            m_run_sem.release(1);
        }

        WorkerThread* victim = nullptr;
        m_dead_pool.pop(&victim);
        delete victim;
        --m_num_threads;
    }

    // Extend
    while (m_num_threads < new_num_threads)
    {      
        if (!m_is_running)
        {
            m_tasks.push({std::make_shared<WorkerThread::BadApplePause>(m_run_sem), PAUSE}); // do i need this ?? 
        }
        new WorkerThread(*this);
        ++m_num_threads;
    }
}
} //ilrd

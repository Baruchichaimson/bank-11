/**************************************
Exercise: thread pool
Date: 03/12/2025
Developer: Baruch Haimson
Reviewer: Guy
Status:	waiting
**************************************/

#include "pool_thread.hpp" 
#include "logger.hpp"

namespace ilrd
{

ThreadPool::WorkerThread::WorkerThread(ThreadPool& pool)
    : m_pool(pool), m_is_alive(true), m_thread(&WorkerThread::RunThread, this)
{
    LOG_DEBUG("WorkerThread ctor entered");
    LOG_DEBUG("WorkerThread ctor exit");
}

ThreadPool::WorkerThread::~WorkerThread()
{
    LOG_DEBUG("WorkerThread dtor entered");
    SetAlive(false);

    if (m_thread.joinable())
    {
        m_thread.join();
    }
    LOG_DEBUG("WorkerThread dtor exit");
}

void ThreadPool::WorkerThread::SetAlive(bool value)
{
    LOG_DEBUG("WorkerThread::SetAlive entered" + std::to_string(value));
    m_is_alive = value;
    LOG_DEBUG("WorkerThread::SetAlive exit");
}

bool ThreadPool::WorkerThread::IsAlive() const
{
    bool alive = m_is_alive;
    return alive;
}

void ThreadPool::WorkerThread::WaitIfPaused()
{
    std::unique_lock<std::mutex> lock(m_pool.m_run_mutex);

    while (!m_pool.m_is_running && IsAlive())
    {
        m_pool.m_run_cond.wait(lock);
    }
    if (!IsAlive()) 
    { 
        LOG_DEBUG("WorkerThread::WaitIfPaused exit - not alive");
        return; 
    }
}

void ThreadPool::WorkerThread::RunThread()
{
    LOG_DEBUG("WorkerThread::RunThread entered");
    TaskEntry entry;

    while (IsAlive())
    {
        WaitIfPaused();

        bool got = m_pool.m_tasks.pop(&entry, std::chrono::milliseconds(100));

        if (!got)
        {
            continue;
        }

        WaitIfPaused();

        entry.second->Execute();
    }
    LOG_DEBUG("WorkerThread::RunThread exit");
}

//================ThreadPool===================

ThreadPool::ThreadPool(std::size_t num_threads): m_is_running(true)
{
    LOG_DEBUG("ThreadPool ctor entered");
    m_threads.reserve(num_threads);

    for (std::size_t i = 0; i < num_threads; ++i)
    {
        m_threads.emplace_back(std::make_unique<WorkerThread>(*this));
    }
    LOG_DEBUG("ThreadPool ctor exit");
}

ThreadPool::~ThreadPool() noexcept
{
    LOG_DEBUG("ThreadPool dtor entered");
    Stop();
    LOG_DEBUG("ThreadPool dtor exit");
}

void ThreadPool::Add(TaskPtr task, priority priority)
{
    LOG_DEBUG("ThreadPool::Add entered");
    m_tasks.push(TaskEntry(priority, task));
    m_run_cond.notify_all();
    LOG_DEBUG("ThreadPool::Add exit");
}

void ThreadPool::Pause()
{
    LOG_DEBUG("ThreadPool::Pause entered");
    m_is_running = false;
    LOG_DEBUG("ThreadPool::Pause exit");
}

void ThreadPool::Resume()
{
    LOG_DEBUG("ThreadPool::Resume entered");
    m_is_running = true;
    m_run_cond.notify_all();
    LOG_DEBUG("ThreadPool::Resume exit");
}

void ThreadPool::Stop()
{
    LOG_DEBUG("ThreadPool::Stop entered");
    TaskEntry task;

    Pause();

    SetNumOfThreads(0);

    while(!m_tasks.empty())
    {
        m_tasks.pop(&task);
    }
    LOG_DEBUG("ThreadPool::Stop exit");
}

void ThreadPool::SetNumOfThreads(std::size_t new_count)
{
    LOG_DEBUG("ThreadPool::SetNumOfThreads entered");
    std::size_t current = m_threads.size();
    std::size_t remove_count = current - new_count;

    if (new_count > current)
    {
        for (std::size_t i = 0; i < new_count - current; ++i)
        {
            m_threads.emplace_back(std::make_unique<WorkerThread>(*this));
        }
    }
    else if (new_count < current)
    {
        for (std::size_t i = 0; i < remove_count; ++i)
        {
            m_threads[i]->SetAlive(false);
        }

        m_run_cond.notify_all();

        m_threads.erase(m_threads.begin(), m_threads.begin() + remove_count);
    }
    LOG_DEBUG("ThreadPool::SetNumOfThreads exit");
}

} // namespace ilrd

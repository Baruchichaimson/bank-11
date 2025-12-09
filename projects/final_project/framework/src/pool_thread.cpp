/**************************************
Exercise: thread pool
Date: 03/12/2025
Developer: Baruch Haimson
Reviewer: Guy
Status:	waiting
**************************************/

#include "pool_thread.hpp" 

namespace ilrd
{

ThreadPool::WorkerThread::WorkerThread(ThreadPool& pool)
    : m_pool(pool), m_is_alive(true), m_thread(&WorkerThread::RunThread, this)
{
}

ThreadPool::WorkerThread::~WorkerThread()
{
    SetAlive(false);

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void ThreadPool::WorkerThread::SetAlive(bool value)
{
    m_is_alive = value;
}

bool ThreadPool::WorkerThread::IsAlive() const
{
    return m_is_alive;
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
        return; 
    }
}

void ThreadPool::WorkerThread::RunThread()
{
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
}

//================ThreadPool===================

ThreadPool::ThreadPool(std::size_t num_threads): m_is_running(true)
{
    m_threads.reserve(num_threads);

    for (std::size_t i = 0; i < num_threads; ++i)
    {
        m_threads.emplace_back(std::make_unique<WorkerThread>(*this));
    }
}

ThreadPool::~ThreadPool() noexcept
{
    Stop();
}

void ThreadPool::Add(TaskPtr task, priority priority)
{
    m_tasks.push(TaskEntry(priority, task));
    m_run_cond.notify_all();
}

void ThreadPool::Pause()
{
    m_is_running = false;
}

void ThreadPool::Resume()
{
    m_is_running = true;
    m_run_cond.notify_all();
}

void ThreadPool::Stop()
{
    TaskEntry task;

    Pause();

    SetNumOfThreads(0);

    while(!m_tasks.empty())
    {
        m_tasks.pop(&task);
    }
}

void ThreadPool::SetNumOfThreads(std::size_t new_count)
{
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
}

} // namespace ilrd

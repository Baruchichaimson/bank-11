/**************************************
Exercise: waitable queue
Date: 01/12/2025
Developer: Baruch Haimson
Reviewer: 
Status:	waiting
**************************************/

#ifndef __ILRD_WAITABLE_QUEUE__
#define __ILRD_WAITABLE_QUEUE__

#include <chrono> /* milliseconds */
#include <condition_variable> /* condition_variable_any */
#include <mutex> /* recursive_timed_mutex */
#include <queue> /* queue<T> */
#include <utility> /* move */

/*
    * container needs to have: push(), pop(), empty(), front()
    * T type has to be copyable/movable
*/

namespace ilrd
{

    template<typename T, typename Container = std::queue<T> >
    class WaitableQueue
    {
    public:
        WaitableQueue() = default;
        
        void push(const T& value);
        void push(T&& value);
        void pop(T* value);
        bool pop(T* value, std::chrono::milliseconds timeout);
        bool empty() const;

        WaitableQueue& operator=(const WaitableQueue& other) = delete;
        WaitableQueue& operator=(const WaitableQueue&& other) = delete;
        WaitableQueue(const WaitableQueue& other) = delete;
        WaitableQueue(const WaitableQueue&& other) = delete;

    private:
        Container m_queue;
        mutable std::recursive_timed_mutex m_mutex; 
        std::condition_variable_any m_cond_var;     
    }; 


    template<typename T, typename Container>
    void WaitableQueue<T, Container>::push(const T& value)
    {
        std::lock_guard<std::recursive_timed_mutex> lock(m_mutex);
        m_queue.push(value);
        m_cond_var.notify_one();
    }

    template<typename T, typename Container>
    void WaitableQueue<T, Container>::push(T&& value)
    {
        {
            std::lock_guard<std::recursive_timed_mutex> lock(m_mutex);
            m_queue.push(std::move(value));
        }
        m_cond_var.notify_one();
    }

    template<typename T, typename Container>
    void WaitableQueue<T, Container>::pop(T* value)
    {
        std::unique_lock<std::recursive_timed_mutex> lock(m_mutex);
        m_cond_var.wait(lock, [this]() 
        { 
            return !m_queue.empty(); 
        });

        *value = std::move(m_queue.front());
        m_queue.pop();
    }

    template<typename T, typename Container>
    bool WaitableQueue<T, Container>::pop(T* value, std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::recursive_timed_mutex> lock(m_mutex);

        bool has_value = m_cond_var.wait_for(lock, timeout, [this]() { return !m_queue.empty(); });

        if (!has_value)
        {
            return false; 
        }

        *value = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    template<typename T, typename Container>
    bool WaitableQueue<T, Container>::empty() const
    {
        std::lock_guard<std::recursive_timed_mutex> lock(m_mutex);
        return m_queue.empty();
    }

} 

#endif // __ILRD_WAITABLE_QUEUE__

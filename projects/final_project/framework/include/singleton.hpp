#ifndef __ILRD_SINGLETON__
#define __ILRD_SINGLETON__

#include <mutex>
#include <atomic>
#include <iostream>
#include <cstdlib>  // std::atexit
#include <new>      // std::nothrow

namespace ilrd
{

template<typename T>
class Singleton
{
public:
    static T* getInstance();

    ~Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    Singleton() = default;

    static void destroyInstanceAtExit();

    static std::atomic<T*> instance;
    static std::atomic<bool> destroyed;
    static std::mutex m_lock;
};

#ifdef __HANDLETON__

template<typename T>
std::atomic<T*> Singleton<T>::instance(nullptr);

template<typename T>
std::atomic<bool> Singleton<T>::destroyed(false);

template<typename T>
std::mutex Singleton<T>::m_lock;

template<typename T>
void Singleton<T>::destroyInstanceAtExit()
{
    T* ptr = instance.load(std::memory_order_acquire);
    if (ptr)
    {
        destroyed.store(true, std::memory_order_release);
        instance.store(nullptr, std::memory_order_relaxed);
        delete ptr;
    }
}

template<typename T>
T* Singleton<T>::getInstance()
{
    if (destroyed.load(std::memory_order_acquire))
    {
        std::cerr << "Error: Singleton used after destruction!" << std::endl;
        std::abort();
    }
    
    T* tmp = instance.load(std::memory_order_acquire);

    if (!tmp)
    {
        std::lock_guard<std::mutex> lock(m_lock);

        tmp = instance.load(std::memory_order_relaxed);
        if (!tmp)
        {
            T* newObj = new T();

            instance.store(newObj, std::memory_order_release);

            if (newObj)
            {
                std::atexit(&Singleton<T>::destroyInstanceAtExit);
            }

            tmp = newObj;
        }
    }

    return tmp;
}

#endif
} // namespace ilrd

#endif // __ILRD_SINGLETON__

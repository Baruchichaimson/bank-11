/**************************************
Exercise: dispatcher
Date: 10/12/2025
Developer: Baruch Haimson
Reviewer: 
Status:	waiting
**************************************/

#ifndef __ILRD_DISPATCHER_HPP__
#define __ILRD_DISPATCHER_HPP__

#include <unordered_set>
#include <memory>

namespace ilrd
{

template <typename EVENT>
    class Dispatcher;


template <typename EVENT>
class BaseCallback
{
public:
    explicit BaseCallback() : m_dispatcher(NULL) {}
    
    virtual ~BaseCallback()
    {
        if (m_dispatcher)
        {
            m_dispatcher->Unsubscribe(this);
        }
    }

    void SetDispatcher(Dispatcher<EVENT>* dispatcher)
    {
        if (m_dispatcher == dispatcher)
            return;

        if (m_dispatcher)
        {
            m_dispatcher->Unsubscribe(this);
        }

        m_dispatcher = dispatcher;
    }

    virtual void Notify(const EVENT& event) = 0;
    virtual void NotifyDeathEx() {}; 
    void NotifyDeath() 
    {
        this->m_dispatcher = NULL;
        NotifyDeathEx();
    }
    
protected:
    Dispatcher<EVENT>* m_dispatcher;
};


template <typename EVENT>
class Dispatcher
{
public:
    Dispatcher() = default;

    ~Dispatcher()
    {
        for (auto cb : m_callbacks)
        {
            cb->NotifyDeath();
        }
    }

    void Subscribe(BaseCallback<EVENT>* callback)
    {
        if (!callback)
            return;

        m_callbacks.insert(callback);
        callback->SetDispatcher(this);
    }

    void Unsubscribe(BaseCallback<EVENT>* callback)
    {
        m_callbacks.erase(callback);
    }

    void Broadcast(const EVENT& event)
    {
        auto snapshot = m_callbacks;

        for (auto cb : snapshot)
        {
            if (m_callbacks.find(cb) != m_callbacks.end())
            {
                cb->Notify(event);
            }
        }
    }

    Dispatcher(const Dispatcher& obj) = delete;
    Dispatcher& operator= (const Dispatcher& obj) = delete;
    Dispatcher(Dispatcher&&) = delete;
    Dispatcher& operator=(Dispatcher&&) = delete;

private:
    std::unordered_set<BaseCallback<EVENT>*> m_callbacks;
};


template <typename EVENT, typename OBSERVER>
class Callback : public BaseCallback<EVENT>
{
public:
    using FuncTypeNotify     = void (OBSERVER::*)(EVENT);
    using FuncTypeNotifyDeath = void (OBSERVER::*)();

    Callback(OBSERVER& observer, FuncTypeNotify notify_func, FuncTypeNotifyDeath notify_death_func)
        : m_observer(observer), m_notify_func(notify_func), m_notify_death_func(notify_death_func)
    {}

    virtual ~Callback() = default;

    virtual void Notify(const EVENT& event) override
    {
        (m_observer.*m_notify_func)(event);  
    }

    virtual void NotifyDeathEx() override
    {
        (m_observer.*m_notify_death_func)();
    }

private:
    OBSERVER& m_observer;
    FuncTypeNotify m_notify_func;
    FuncTypeNotifyDeath m_notify_death_func;
};


} // namespace ilrd

#endif // __ILRD_DISPATCHER_HPP__

/**************************************
Exercise: dispatcher
Date: 10/12/2025
Developer: Baruch Haimson
Reviewer: Ben
Status:	waiting
**************************************/

#ifndef __ILRD_DISPATCHER_HPP__
#define __ILRD_DISPATCHER_HPP__

#include <unordered_set> // unordered_set
#include <algorithm>  // for_each

namespace ilrd
{

template <typename EVENT>
    class Dispatcher;


template <typename EVENT>
class BaseCallback
{
public:
    explicit BaseCallback();
    virtual ~BaseCallback();
    void SetDispatcher(Dispatcher<EVENT>* dispatcher);
    virtual void Notify(const EVENT& event) = 0;
    virtual void NotifyDeathEx(); 
    void NotifyDeath();
    
protected:
    Dispatcher<EVENT>* m_dispatcher;
};


template <typename EVENT>
class Dispatcher
{
public:
    Dispatcher() = default;
    ~Dispatcher();
    void Subscribe(BaseCallback<EVENT>* callback);
    void Unsubscribe(BaseCallback<EVENT>* callback);
    void Broadcast(const EVENT& event);

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

    Callback(OBSERVER& observer, FuncTypeNotify notify_func, FuncTypeNotifyDeath notify_death_func);

    virtual ~Callback() = default;
    virtual void Notify(const EVENT& event);
    virtual void NotifyDeathEx();

private:
    OBSERVER& m_observer;
    FuncTypeNotify m_notify_func;
    FuncTypeNotifyDeath m_notify_death_func;
};

/************************ BaseCallback ************************************** */

template <typename EVENT>
BaseCallback<EVENT>::BaseCallback() : m_dispatcher(NULL) 
{}

template <typename EVENT>
BaseCallback<EVENT>::~BaseCallback()
{
    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
    }
}

template <typename EVENT>
void BaseCallback<EVENT>::SetDispatcher(Dispatcher<EVENT>* dispatcher)
{
    if (m_dispatcher == dispatcher)
        return;

    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
    }

    m_dispatcher = dispatcher;
}

template <typename EVENT>
void BaseCallback<EVENT>::NotifyDeathEx() 
{}

template <typename EVENT>
void BaseCallback<EVENT>::NotifyDeath() 
{
    this->SetDispatcher(NULL);
    NotifyDeathEx();
}

/************************ Dispatcher ************************************** */


template <typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
   std::for_each(m_callbacks.begin(), m_callbacks.end(), [](BaseCallback<EVENT>* cb) 
   {
       cb->NotifyDeath();
   });
}

template <typename EVENT>
void Dispatcher<EVENT>::Subscribe(BaseCallback<EVENT>* callback)
{
    m_callbacks.insert(callback);
    callback->SetDispatcher(this);
}

template <typename EVENT>
void Dispatcher<EVENT>::Unsubscribe(BaseCallback<EVENT>* callback)
{
    m_callbacks.erase(callback);
}

template <typename EVENT>
void Dispatcher<EVENT>::Broadcast(const EVENT& event)
{
    std::unordered_set<BaseCallback<EVENT>*> callbacks_copy(m_callbacks);
    std::for_each(callbacks_copy.begin(), callbacks_copy.end(), [&event](BaseCallback<EVENT>* callback){ callback->Notify(event);});
}

/************************ Callback ************************************** */

template <typename EVENT, typename OBSERVER>
Callback<EVENT,OBSERVER>::Callback(OBSERVER& observer, FuncTypeNotify notify_func, FuncTypeNotifyDeath notify_death_func)
        : m_observer(observer), m_notify_func(notify_func), m_notify_death_func(notify_death_func)
    {}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::Notify(const EVENT& event)
{
    (m_observer.*m_notify_func)(event);  
}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::NotifyDeathEx()
{
    (m_observer.*m_notify_death_func)();
}

} // namespace ilrd

#endif // __ILRD_DISPATCHER_HPP__

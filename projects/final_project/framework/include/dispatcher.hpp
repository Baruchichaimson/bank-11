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
#include "logger.hpp"

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
{
    LOG_DEBUG("BaseCallback ctor entered");
    LOG_DEBUG("BaseCallback ctor exit");
}

template <typename EVENT>
BaseCallback<EVENT>::~BaseCallback()
{
    LOG_DEBUG("BaseCallback dtor entered");
    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
    }
    LOG_DEBUG("BaseCallback dtor exit");
}

template <typename EVENT>
void BaseCallback<EVENT>::SetDispatcher(Dispatcher<EVENT>* dispatcher)
{
    LOG_DEBUG("BaseCallback::SetDispatcher entered");
    if (m_dispatcher == dispatcher)
    {
        LOG_DEBUG("BaseCallback::SetDispatcher exit");
        return;
    }

    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
    }

    m_dispatcher = dispatcher;
    LOG_DEBUG("BaseCallback::SetDispatcher exit");
}

template <typename EVENT>
void BaseCallback<EVENT>::NotifyDeathEx() 
{
    LOG_DEBUG("BaseCallback::NotifyDeathEx entered");
    LOG_DEBUG("BaseCallback::NotifyDeathEx exit");
}

template <typename EVENT>
void BaseCallback<EVENT>::NotifyDeath() 
{
    LOG_DEBUG("BaseCallback::NotifyDeath entered");
    this->SetDispatcher(NULL);
    NotifyDeathEx();
    LOG_DEBUG("BaseCallback::NotifyDeath exit");
}

/************************ Dispatcher ************************************** */


template <typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
   LOG_DEBUG("Dispatcher dtor entered");
   std::for_each(m_callbacks.begin(), m_callbacks.end(), [](BaseCallback<EVENT>* cb) 
   {
       cb->NotifyDeath();
   });
   LOG_DEBUG("Dispatcher dtor exit");
}

template <typename EVENT>
void Dispatcher<EVENT>::Subscribe(BaseCallback<EVENT>* callback)
{
    LOG_DEBUG("Dispatcher::Subscribe entered");
    m_callbacks.insert(callback);
    callback->SetDispatcher(this);
    LOG_DEBUG("Dispatcher::Subscribe exit");
}

template <typename EVENT>
void Dispatcher<EVENT>::Unsubscribe(BaseCallback<EVENT>* callback)
{
    LOG_DEBUG("Dispatcher::Unsubscribe entered");
    m_callbacks.erase(callback);
    LOG_DEBUG("Dispatcher::Unsubscribe exit");
}

template <typename EVENT>
void Dispatcher<EVENT>::Broadcast(const EVENT& event)
{
    LOG_DEBUG("Dispatcher::Broadcast entered");
    std::unordered_set<BaseCallback<EVENT>*> callbacks_copy(m_callbacks);
    std::for_each(callbacks_copy.begin(), callbacks_copy.end(), [&event](BaseCallback<EVENT>* callback){ callback->Notify(event);});
    LOG_DEBUG("Dispatcher::Broadcast exit");
}

/************************ Callback ************************************** */

template <typename EVENT, typename OBSERVER>
Callback<EVENT,OBSERVER>::Callback(OBSERVER& observer, FuncTypeNotify notify_func, FuncTypeNotifyDeath notify_death_func)
        : m_observer(observer), m_notify_func(notify_func), m_notify_death_func(notify_death_func)
    {
        LOG_DEBUG("Callback ctor entered");
        LOG_DEBUG("Callback ctor exit");
    }

template <typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::Notify(const EVENT& event)
{
    LOG_DEBUG("Callback::Notify entered");
    (m_observer.*m_notify_func)(event);  
    LOG_DEBUG("Callback::Notify exit");
}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::NotifyDeathEx()
{
    LOG_DEBUG("Callback::NotifyDeathEx entered");
    (m_observer.*m_notify_death_func)();
    LOG_DEBUG("Callback::NotifyDeathEx exit");
}

} // namespace ilrd

#endif // __ILRD_DISPATCHER_HPP__

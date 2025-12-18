/**************************************
Exercise: 	final project - Async Injection 
Date:		18/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#include "asyncinjection.hpp"
#include "handleton.hpp"

namespace ilrd
{
AsyncInjection::AsyncInjection(std::function<bool(void)> action, std::chrono::milliseconds interval) 
    : m_action_func(std::move(action)), m_interval(interval), m_task(std::make_shared<AsyncTask>(*this))
{
   Add();
}

AsyncInjection::AsyncTask::AsyncTask(AsyncInjection& father)
    : m_father(father)
{
}

void AsyncInjection::AsyncTask::Execute()
{
    m_father.PreformAct();
}

void AsyncInjection::PreformAct()
{
    if (m_action_func())
    {
        delete this; 
    }
    else
    {
        Add();
    }
}

void AsyncInjection::Add()
{
    Handleton<Scheduler>::GetInstance().Add(m_task, m_interval);
}
}

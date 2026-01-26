/**************************************
Exercise: 	final project - Async Injection 
Date:		18/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#include "asyncinjection.hpp"
#include "handleton.hpp"
#include "logger.hpp"

namespace ilrd
{
AsyncInjection::AsyncInjection(std::function<bool(void)> action, std::chrono::milliseconds interval) 
    : m_action_func(std::move(action)), m_interval(interval), m_task(std::make_shared<AsyncTask>(*this))
{
    LOG_DEBUG("AsyncInjection ctor entered");
    Add();
    LOG_DEBUG("AsyncInjection ctor exit");
}

AsyncInjection::AsyncTask::AsyncTask(AsyncInjection& father)
    : m_father(father)
{
    LOG_DEBUG("AsyncTask ctor entered");
    LOG_DEBUG("AsyncTask ctor exit");
}

void AsyncInjection::AsyncTask::Execute()
{
    LOG_DEBUG("AsyncTask::Execute entered");
    m_father.PreformAct();
    LOG_DEBUG("AsyncTask::Execute exit");
}

void AsyncInjection::PreformAct()
{
    LOG_DEBUG("AsyncInjection::PreformAct entered");
    if (m_action_func())
    {
        delete this; 
    }
    else
    {
        Add();
    }
    LOG_DEBUG("AsyncInjection::PreformAct exit");
}

void AsyncInjection::Add()
{
    LOG_DEBUG("AsyncInjection::Add entered");
    Handleton<Scheduler>::GetInstance().Add(m_task, m_interval);
    LOG_DEBUG("AsyncInjection::Add exit");
}
}

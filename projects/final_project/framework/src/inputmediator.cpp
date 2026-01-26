/**************************************
Exercise: 	final project - input mediator
Date:		23/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/
#include "reactor.hpp"
#include "factory.hpp"
#include "icommand.hpp"
#include "handleton.hpp"
#include "singleton.hpp"
#include "asyncinjection.hpp"
#include "inputmediator.hpp"
#include "pool_thread.hpp"
#include "logger.hpp"

namespace ilrd
{
InputMediator::InputMediator(const std::vector<std::tuple<int, Reactor::Mode, input_proxy_t>>& registrations, std::shared_ptr<Reactor::IListener> listener) : m_reactor(listener)
{
    LOG_DEBUG("InputMediator ctor entered");

    for (const auto& [fd, mode, proxy] : registrations)
    {
        m_reactor.Add(fd, mode, ReactorCallback(proxy));
    }
    LOG_DEBUG("InputMediator ctor exit");
}

void InputMediator::Run()
{
    LOG_DEBUG("InputMediator::Run entered");
    m_reactor.Run();
    LOG_DEBUG("InputMediator::Run exit");
}
void InputMediator::Stop()
{
    m_reactor.Stop();
}

/**********************TPTask**********************************/

InputMediator::TPTask::TPTask(std::shared_ptr<IKeyArgs> args): m_args(std::move(args))
{
    LOG_DEBUG("TPTask ctor entered");
    LOG_DEBUG("TPTask ctor exit");
}

InputMediator::TPTask::~TPTask()
{
    LOG_DEBUG("TPTask dtor entered");
    LOG_DEBUG("TPTask dtor exit");
}

void InputMediator::TPTask::Execute()
{
    LOG_DEBUG("TPTask::Execute entered");
    if (!m_args)
    {
        LOG_DEBUG("TPTask::Execute exit - empty args");
        return;
    }
    std::shared_ptr<ICommand> command = Handleton<Factory<ICommand, int>>::GetInstance().Create(m_args->GetKey());
    ICommand::async_args args = command->Execute(m_args);

    if(args.first)
    {
        new AsyncInjection(args.first, args.second);
    }
    LOG_DEBUG("TPTask::Execute exit");
}

/**********************ReactorCallback**********************************/

InputMediator::ReactorCallback::ReactorCallback( std::shared_ptr<IInputProxy> inputProxy) : m_input_proxy(std::move(inputProxy))
{
    LOG_DEBUG("ReactorCallback ctor entered");
    LOG_DEBUG("ReactorCallback ctor exit");
}

void InputMediator::ReactorCallback::operator()(int fd, Reactor::Mode mode)
{
    LOG_DEBUG("ReactorCallback::operator() entered");
    auto args = m_input_proxy->GetKeyArgs(fd, mode);
    if (!args)
    {
        LOG_DEBUG("ReactorCallback::operator() exit - no args");
        return;
    }
   
    Handleton<ThreadPool>::GetInstance().Add(std::make_shared<TPTask>(args));
    
    LOG_DEBUG("ReactorCallback::operator() exit");
}
}

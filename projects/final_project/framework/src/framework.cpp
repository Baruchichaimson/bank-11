/**************************************
Exercise: 	final project - framework
Date:		24/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#include "framework.hpp"
#include "handleton.hpp"
#include "factory.hpp"

namespace ilrd
{

FrameWork::FrameWork(const std::vector<CallBacks>& call_backs, std::shared_ptr<Reactor::IListener> listener, const std::vector<Commands>& commands, const std::string& plugin_dir)
    : m_mediator(call_backs, listener), m_dir_monitor(plugin_dir),
      m_loader(), m_cb(m_loader, &DllLoader::LoadCallback, &DllLoader::Cleanup)
{
    LOG_DEBUG("Framework ctor ENTER");
    for (const auto& cmd : commands)
    {
        Handleton<Factory<ICommand, int>>::GetInstance().Add(cmd.first, cmd.second);
    }

    m_dir_monitor.Register(&m_cb);

    LOG_DEBUG("Framework ctor EXIT");
}

void FrameWork::Run()
{
    LOG_DEBUG("Framework Run ENTER");

    m_dir_monitor.Run();
    m_mediator.Run();

    LOG_DEBUG("Framework Run EXIT");

}

void FrameWork::Stop() 
{
    LOG_DEBUG("Framework dtor ENTER");

    m_dir_monitor.Unregister(&m_cb);
    m_mediator.Stop();

    LOG_DEBUG("Framework dtor EXIT");
}

} // namespace ilrd

/**************************************
Exercise: 	final project - framework
Date:		24/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/

#ifndef __ILRD_FRAMEWORK__
#define __ILRD_FRAMEWORK__

#include <functional>   // std::function
#include <memory>       // std::shared_ptr
#include <string>       // std::string
#include <tuple>        // std::tuple
#include <vector>       // std::vector

#include "reactor.hpp"
#include "inputmediator.hpp"
#include "dirmonitor.hpp"
#include "dllloader.hpp"
#include "icommand.hpp"

namespace ilrd
{

class FrameWork
{
public:
    using CallBacks = std::tuple<int, Reactor::Mode, InputMediator::input_proxy_t>;
    using Commands = std::pair<int, std::function<std::shared_ptr<ICommand>()>>;
    
    FrameWork(const std::vector<CallBacks>& call_backs, 
                std::shared_ptr<Reactor::IListener> listener, 
                const std::vector<Commands>& commands,
                const std::string& plugin_dir);
                
    ~FrameWork() = default;
    
    void Run();
    void Stop();

private:
    InputMediator m_mediator;
    DirMonitor m_dir_monitor;
    DllLoader m_loader;
    Callback<const std::string&, DllLoader> m_cb;

}; // class FrameWork

} // ilrd

#endif  // __ILRD_FRAMEWORK__    

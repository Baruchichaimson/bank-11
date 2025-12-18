/**************************************
Exercise: 	final project - reactor
Date:		16/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/
#ifndef __ILRD_SELECT_LISTENER_HPP__
#define __ILRD_SELECT_LISTENER_HPP__

#include <vector>
#include <sys/select.h>

#include "reactor.hpp"

namespace ilrd
{

class SelectListener : public Reactor::IListener
{
public:
    SelectListener() = default;
    ~SelectListener() override = default;

    std::vector<Reactor::fd_pair> Listen(const std::vector<Reactor::fd_pair>& set) override;
}; // class SelectListener

} // namespace ilrd

#endif // __ILRD_SELECT_LISTENER_HPP__

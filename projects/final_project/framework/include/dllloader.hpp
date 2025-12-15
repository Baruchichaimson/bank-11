#ifndef __ILRD_DLL_LOADER_HPP__
#define __ILRD_DLL_LOADER_HPP__

#include <string>
#include "dispatcher.hpp"

namespace ilrd
{

class DllLoader
{
public:
    DllLoader() = default;
    ~DllLoader() noexcept = default;

    void* Load(const std::string& plugin_name);
    void LoadCallback(const std::string& plugin_name);

private:
};

} // namespace ilrd

#endif // __ILRD_DLL_LOADER_HPP__

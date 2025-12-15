#include <dlfcn.h>
#include <stdexcept>
#include <iostream>

#include "dllloader.hpp"

namespace ilrd
{

void* DllLoader::Load(const std::string& plugin_path)
{
    void* handle = dlopen(plugin_path.c_str(), RTLD_LAZY);
    if (!handle)
    {
        throw std::runtime_error(std::string("dlopen failed: ") + dlerror());
    }


    return handle;
}

void DllLoader::LoadCallback(const std::string& plugin_path)
{
    Load(plugin_path);
}

} // namespace ilrd

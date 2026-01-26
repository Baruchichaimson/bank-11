#include <dlfcn.h>
#include <filesystem>
#include <stdexcept>
#include <iostream>

#include "dllloader.hpp"
#include "logger.hpp"

namespace ilrd
{

void* DllLoader::Load(const std::string& plugin_path)
{
    LOG_DEBUG("DllLoader::Load entered");
    void* handle = dlopen(plugin_path.c_str(), RTLD_LAZY);
    if (!handle)
    {
        throw std::runtime_error(std::string("dlopen failed: ") + dlerror());
    }


    LOG_DEBUG("DllLoader::Load exit");
    return handle;
}

void DllLoader::LoadCallback(const std::string& plugin_path)
{
    LOG_DEBUG("DllLoader::LoadCallback entered");
    if (std::filesystem::path(plugin_path).extension() != ".so")
    {
        LOG_INFO("DllLoader::LoadCallback skipping non-.so file: " + plugin_path);
        LOG_DEBUG("DllLoader::LoadCallback exit");
        return;
    }

    try
    {
        Load(plugin_path);
    }
    catch (const std::exception& e)
    {
        LOG_INFO(std::string("DllLoader::LoadCallback failed for ") + plugin_path + ": " + e.what());
    }

    LOG_DEBUG("DllLoader::LoadCallback exit");
}

void DllLoader::Cleanup()
{
    LOG_DEBUG("DllLoader::Cleanup entered");
    LOG_DEBUG("DllLoader::Cleanup exit");
}

} // namespace ilrd

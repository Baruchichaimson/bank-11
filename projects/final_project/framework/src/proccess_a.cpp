#include <iostream>
#include <dlfcn.h>
#include "singleton.hpp"

extern template class ilrd::Singleton<int>;

int main() 
{
    int* instance = ilrd::Singleton<int>::getInstance();
    std::cout << "[main] Singleton<int> address: " << instance << std::endl;

    // Load plugin
    void* handle = dlopen("./libb.so", RTLD_NOW);
    if (!handle) {
        std::cerr << "[main] Failed to load plugin: " << dlerror() << std::endl;
        return 1;
    }

    // Reset errors
    dlerror();

    using PluginEntry = void(*)();
    PluginEntry entry = (PluginEntry)dlsym(handle, "Foo");

    const char* error = dlerror();
    if (error != nullptr) {
        std::cerr << "[main] dlsym error: " << error << std::endl;
        dlclose(handle);
        return 1;
    }

    entry();

    dlclose(handle);
    return 0;
}

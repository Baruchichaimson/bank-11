// Simple demo plugin: prints when the .so is loaded.
#include <iostream>

__attribute__((constructor))
static void OnLoad()
{
    std::cout << "[sample_plugin] loaded" << std::endl;
}

extern "C" void Ping()
{
    std::cout << "[sample_plugin] Ping()" << std::endl;
}

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>

#include "dirmonitor.hpp"
#include "dllloader.hpp"
#include "dispatcher.hpp"

using namespace ilrd;

// ---------------- Callback for DirMonitor → DllLoader ----------------
class LoaderObserver
{
public:
    LoaderObserver(DllLoader& loader)
    : m_loader(loader)
    {
    }

    void OnEvent(const std::string& plugin_path)
    {
        std::cout << "[DirMonitor EVENT] " << plugin_path << std::endl;

        std::string::size_type pos = plugin_path.find(':');
        if (pos == std::string::npos || pos + 1 >= plugin_path.size())
        {
            return;
        }

        std::string real_path = plugin_path.substr(pos + 1);

        // load plugin
        m_loader.LoadCallback(real_path);
    }

    void OnDeath()
    {
        std::cout << "[Observer] DEAD\n";
    }

private:
    DllLoader& m_loader;
};

// ---------------------------------------------------------------------

int main()
{
    system("mkdir -p plugins");

    std::cout << "=== Starting FULL SYSTEM TEST ===\n";

    DllLoader loader;
    DirMonitor monitor("plugins");

    LoaderObserver obs(loader);

    Callback<const std::string&, LoaderObserver> cb(
        obs,
        &LoaderObserver::OnEvent,
        &LoaderObserver::OnDeath
    );

    monitor.Register(&cb);
    monitor.Run();

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // Create dummy plugin
    std::cout << "Copying plugin_dummy.so → plugins/...\n";
    system("cp test/plugin_dummy.so plugins/plugin_dummy.so");

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    // Delete plugin to emit a REMOVE event
    std::cout << "Removing plugin_dummy.so from plugins/...\n";
    system("rm -f plugins/plugin_dummy.so");

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    monitor.Stop();

    std::cout << "=== FULL SYSTEM TEST FINISHED ===\n";
    return 0;
}

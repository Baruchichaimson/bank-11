#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <thread>
#include <unistd.h>     // STDIN_FILENO, read
#include <chrono>

#include "framework.hpp"
#include "factory.hpp"
#include "handleton.hpp"
#include "pool_thread.hpp"
#include "iinputproxy.hpp"
#include "ikeyargs.hpp"
#include "logger.hpp"
#include "dirmonitor.hpp"
#include "select_listener.hpp"

/*
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude test/framework_test.cpp
 src/framework.cpp src/inputmediator.cpp src/reactor.cpp src/pool_thread.cpp 
 src/handleton.cpp src/scheduler.cpp src/asyncinjection.cpp src/logger.cpp 
 src/dirmonitor.cpp src/dllloader.cpp src/select_listener.cpp -pthread -ldl
*/
using namespace ilrd;

enum CommandKey
{
    READ_CMD = 0,
    WRITE_CMD = 1
};

FrameWork* g_framework = nullptr;

// ====================== KeyArgs Stub ======================
class ReadKeyArgs : public IKeyArgs
{
public:
    explicit ReadKeyArgs(int key):m_key(key) {}
    ~ReadKeyArgs() = default;

    int GetKey() override
    {
        return m_key;
    }

private:
    int m_key;
};

// ====================== InputProxy Stub ======================

class NDBPoxy : public IInputProxy
{
public:
    NDBPoxy() = default;
    ~NDBPoxy() = default;

    std::shared_ptr<IKeyArgs> GetKeyArgs(int fd, Reactor::Mode mode) override 
    {
        char buf[256] = {0};
        if (read(fd, buf, sizeof(buf) - 1) <= 0) {
            return std::make_shared<ReadKeyArgs>(mode);
        }

        std::string input(buf);
        if (input.find("quit") == 0 && g_framework) 
        {
            g_framework->Stop();
            return nullptr; 
        }

        if (input.find("100") != std::string::npos)
        {
            return std::make_shared<ReadKeyArgs>(100);
        }

        return std::make_shared<ReadKeyArgs>(mode);
    }
};

// ====================== Commands Stub ======================
class StubReadCommand : public ICommand
{
public:
    async_args Execute(std::shared_ptr<IKeyArgs> task_args) override
    {
        (void)task_args;
        std::cout << "[StubReadCommand] Execute called!" << std::endl;

        return {[]() { LOG_INFO("mession"); return true; }, std::chrono::milliseconds(0)};
    }
};

class StubWriteCommand : public ICommand
{
public:
    async_args Execute(std::shared_ptr<IKeyArgs> task_args) override
    {
        (void)task_args;
        std::cout << "[StubWriteCommand] Execute called!" << std::endl;

        return {[]() { return true; }, std::chrono::milliseconds(0)};
    }
};

// ====================== MAIN ======================
int main()
{    
    std::cout << "=== Framework Test ===" << std::endl;
    LOG_DEBUG("================================ Framework Test ================================");

    std::vector<FrameWork::Commands> commands = 
    {
        {READ_CMD,  []() { return std::make_shared<StubReadCommand>(); }},
        {WRITE_CMD, []() { return std::make_shared<StubWriteCommand>(); }},
    };

    auto listener = std::make_shared<SelectListener>();

    auto proxy = std::make_shared<NDBPoxy>();

    std::vector<FrameWork::CallBacks> entries =
    {
        FrameWork::CallBacks(STDIN_FILENO, Reactor::READ, proxy)
    };

    const std::string plugin_dir = "./plugins";
    std::filesystem::create_directories(plugin_dir);

    FrameWork fw(entries, listener, commands, plugin_dir);

    g_framework = &fw;

    std::cout << "Type something and press Enter (type 'quit' to exit):" << std::endl;

    fw.Run();

    Handleton<ThreadPool>::GetInstance().Stop();

    std::cout << "=== Test End ===" << std::endl;
    
    LOG_INFO("================================ Test End ================================");


    return 0; 
}

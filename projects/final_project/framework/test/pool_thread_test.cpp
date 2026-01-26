/**************************************
Exercise: thread pool
Date: 03/12/2025
Developer: Baruch Haimson
Reviewer: Guy
Status:	waiting
**************************************/

/* g++ -std=c++20 -Wall -Wextra -pthread -Iinclude src/thread_pool.cpp src/logger.cpp src/handleton.cpp src/scheduler.cpp test/pool_thread_test.cpp */

#include <iostream>     // std::cout
#include <unistd.h>     // sleep
#include <memory>       // std::make_shared
#include <iostream>
#include <unistd.h>
#include <memory>

#include "thread_pool.hpp" // ThreadPool

using namespace ilrd;

class TPTaskPrint : public ThreadPool::ITPTask
{
public:
    TPTaskPrint(const std::string& msg) : m_msg(msg) {}
    void Execute() override
    {
        std::cout << m_msg << std::endl;
    }

private:
    std::string m_msg;
};

inline ThreadPool::task MakeTask(const std::string& msg,
                                 ThreadPool::Priority pr = ThreadPool::MEDIUM)
{
    return std::make_pair(std::make_shared<TPTaskPrint>(msg), pr);
}

void BasicRunTest();
void ResizeTest();
void PauseResumeTest();
void StopTest();

int main()
{
    std::cout << "***** ThreadPool Test Start *****" << std::endl;

    BasicRunTest();
    ResizeTest();
    PauseResumeTest();
    StopTest();

    std::cout << "\n***** Great Success!!! *****" << std::endl;

    return 0;
}

/***************************
        TESTS
****************************/

void BasicRunTest()
{
    std::cout << "\n--- BasicRunTest ---" << std::endl;

    ThreadPool tp(2);
    tp.Run();
    
    tp.Add(MakeTask("Hello from TP!"));
    tp.Add(MakeTask("Task #2"));
    tp.Add(MakeTask("Task #3"));

    sleep(1);
}

void ResizeTest()
{
    std::cout << "\n--- ResizeTest ---" << std::endl;

    ThreadPool tp(2);
    tp.Run();

    std::cout << "Adding tasks before resize..." << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        tp.Add(MakeTask("Before resize #" + std::to_string(i)));
    }

    sleep(1);

    std::cout << "Resizing to 4 threads..." << std::endl;
    tp.SetNumOfThreads(4);

    for (int i = 0; i < 5; ++i)
    {
        tp.Add(MakeTask("After grow #" + std::to_string(i)));
    }

    sleep(1);

    std::cout << "Resizing to 1 thread..." << std::endl;
    tp.SetNumOfThreads(1);

    tp.Add(MakeTask("After shrink (1 thread left)"));

    sleep(1);
}

void PauseResumeTest()
{
    std::cout << "\n--- PauseResumeTest ---" << std::endl;

    ThreadPool tp(3);
    tp.Run();

    tp.Add(MakeTask("Task 1"));
    tp.Add(MakeTask("Task 2"));
    tp.Add(MakeTask("Task 3"));

    sleep(1);

    std::cout << "Pausing..." << std::endl;
    tp.Pause();

    tp.Add(MakeTask("This waits until resume"));
    tp.Add(MakeTask("This too"));

    std::cout << "Resuming..." << std::endl;
    tp.Run();

    sleep(1);
}

void StopTest()
{
    std::cout << "\n--- StopTest ---" << std::endl;

    ThreadPool tp(3);
    tp.Run();

    tp.Add(MakeTask("Final #1"));
    tp.Add(MakeTask("Final #2"));
    tp.Add(MakeTask("Final #3"));

    sleep(1);

    std::cout << "Stopping..." << std::endl;
    tp.Stop();

    tp.Add(MakeTask("Final #1"));
    tp.Add(MakeTask("Final #2"));
    tp.Add(MakeTask("Final #3"));

    std::cout << "ThreadPool stopped successfully." << std::endl;
}


/**************************************
Exercise: thread pool
Date: 03/12/2025
Developer: Baruch Haimson
Reviewer: Guy
Status:	waiting
**************************************/

#include <iostream> // printf
#include <memory>   // make_shared
#include <chrono>   // milliseconds
#include <thread>   // sleep_for

#include "pool_thread.hpp" // ThreadPool

using namespace ilrd;

// ----- Example Task -----
class PrintTask : public ThreadPool::ITPTask
{
public:
    explicit PrintTask(int n)
        : m_n(n)
    {}

    void Execute() override
    {
        std::cout << "Executing task: " << m_n
                << "  (Thread: " << std::this_thread::get_id() << ")\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

private:
    int m_n;
};

int main()
{
    std::cout << "Creating thread pool with 3 threads...\n";

    ThreadPool pool(3);

    std::cout << "Adding 10 tasks...\n";

    for (int i = 1; i <= 10; ++i)
    {
        pool.Add(std::make_shared<PrintTask>(i), ThreadPool::MEDIUM);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));


    std::cout << "\nIncreasing threads to 5...\n";

    pool.SetNumOfThreads(5);

    for (int i = 11; i <= 20; ++i)
    {
        pool.Add(std::make_shared<PrintTask>(i), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));


    std::cout << "\nPausing pool...\n";
    pool.Pause();

    std::cout << "Pool paused. Adding tasks (#21-#25)...\n";

    for (int i = 21; i <= 25; ++i)
    {
        pool.Add(std::make_shared<PrintTask>(i), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));


    std::cout << "\nResuming pool...\n";
    pool.Resume();

    std::this_thread::sleep_for(std::chrono::seconds(3));


    std::cout << "\nDecreasing threads to 2...\n";
    pool.SetNumOfThreads(2);

    std::this_thread::sleep_for(std::chrono::seconds(2));


    for (int i = 26; i <= 30; ++i)
    {
        pool.Add(std::make_shared<PrintTask>(i), ThreadPool::HIGH);
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "\nStopping pool...\n";
    pool.Stop();

    std::cout << "Thread pool stopped cleanly.\n";

    return 0;
}

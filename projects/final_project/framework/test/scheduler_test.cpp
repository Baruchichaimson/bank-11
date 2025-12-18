#include "scheduler.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

using namespace ilrd;

class TestTask : public Scheduler::ISTask
{
public:
    TestTask(std::atomic<int>& counter, int val) 
        : m_counter(counter), m_val(val) {}

    void Execute() override
    {
        std::cout << "Task executed: " << m_val << std::endl;
        m_counter++;
        std::cout << "Counter value is: " << m_counter <<std::endl;
    }

private:
    std::atomic<int>& m_counter;
    int m_val;
};

int main()
{
    ilrd::Scheduler& sched = ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    std::atomic<int> counter{0};

    sched.Add(std::make_shared<TestTask>(counter, 3), std::chrono::milliseconds(1000));
    sched.Add(std::make_shared<TestTask>(counter, 1), std::chrono::milliseconds(100));
    sched.Add(std::make_shared<TestTask>(counter, 2), std::chrono::milliseconds(300));

    std::cout << "Counter: " << counter <<std::endl;

    std::cout << "Waiting for tasks..." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "Counter: " << counter <<std::endl;
    if (counter == 3)
    {
        std::cout << "All tasks executed successfully!" << std::endl;
    }
    else
    {
        std::cout << "Error: Not all tasks executed!" << std::endl;
    }

    return 0;
}

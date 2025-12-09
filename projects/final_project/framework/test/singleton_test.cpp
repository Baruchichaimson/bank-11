#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <cstdlib>

#include "singleton.hpp"

using namespace ilrd;

class MySingleton
{
public:
    void setValue(int v) { value = v; }
    int getValue() const { return value; }

private:

    friend class Singleton<MySingleton>;
    MySingleton() : value(0) { std::cout << "MySingleton ctor\n"; }
    ~MySingleton() { std::cout << "MySingleton dtor\n"; }

    int value;
};

void threadFunc(int id)
{
    MySingleton& s = Singleton<MySingleton>::getInstance();
    s.setValue(id);
    std::cout << "Thread " << id << " value: " << s.getValue() << "\n";
}



int main()
{
    std::cout << "Test 1: Single-thread creation\n";
    MySingleton& s1 = Singleton<MySingleton>::getInstance();
    s1.setValue(42);
    assert(s1.getValue() == 42);

    std::cout << "Test 2: Multi-thread access\n";
    std::vector<std::thread> threads;
    for (int i = 1; i <= 5; ++i)
    {
        threads.emplace_back(threadFunc, i);
    }
    for (auto& t : threads)
    {
        t.join();
    }

    MySingleton& sFinal = Singleton<MySingleton>::getInstance();
    std::cout << "Final value: " << sFinal.getValue() << "\n";

    std::cout << "Test 3: Access after destruction\n"; 
    std::cout << "All tests passed\n";
    return 0;
}
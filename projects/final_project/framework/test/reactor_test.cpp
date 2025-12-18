/**************************************
Exercise: 	final project - reactor
Date:		16/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/
#include <iostream> // std::cout
#include <unistd.h> // close

#include "reactor.hpp"
#include "select_listener.hpp"

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED "\033[1;31m"

using namespace ilrd;

void TestBasic();
void TestRemove();
void TestStop();
template <typename T>
static void PrintValidity(T result, T expected_result, std::string test_name);

int main()
{
    TestBasic();
    TestRemove();
    TestStop();

    return 0;
}

void TestBasic()
{
    std::cout << "TestBasic:" << std::endl;
    std::cout << "---------" << std::endl;

    int pipefd[2];
    pipe(pipefd);

    int read_fd = pipefd[0];
    int write_fd = pipefd[1];

    auto listener = std::make_shared<SelectListener>();
    Reactor reactor(listener);

    bool read_called = false;

    reactor.Add(read_fd, Reactor::Mode::READ,
        [&](int fd, Reactor::Mode)
        {
            char c;
            read(fd, &c, 1);
            PrintValidity(c, 'A', "got A");

            read_called = true;

            reactor.Remove(fd, Reactor::READ);
        });

    write(write_fd, "A", 1);
    write(write_fd, "B", 1);

    reactor.Run();

    PrintValidity(read_called, true, "read_called");

    close(read_fd);
    close(write_fd);

    std::cout << std::endl;
}

void TestRemove()
{
    std::cout << "TestRemove:" << std::endl;
    std::cout << "-----------" << std::endl;

    int pipefd[2];
    pipe(pipefd);

    int read_fd = pipefd[0];
    int write_fd = pipefd[1];

    auto listener = std::make_shared<SelectListener>();
    Reactor reactor(listener);

    bool first_called = false;
    bool second_called = false;

    reactor.Add(read_fd, Reactor::READ,
        [&](int fd, Reactor::Mode)
        {
            char c;
            read(fd, &c, 1);
            PrintValidity(c, 'A', "got A");

            first_called = true;

            // remove second before it runs
            reactor.Remove(fd, Reactor::WRITE);

            reactor.Stop();
        });

    reactor.Add(read_fd, Reactor::WRITE,
        [&](int, Reactor::Mode)
        {
            second_called = true;
        });

    write(write_fd, "A", 1);

    reactor.Run();

    PrintValidity(first_called, true, "first_called");
    PrintValidity(second_called, false, "second_called in first callback");

    close(read_fd);
    close(write_fd);

    std::cout << std::endl;
}

void TestStop()
{
    std::cout << "TestStop:" << std::endl;
    std::cout << "---------" << std::endl;

    int pipefd[2];
    pipe(pipefd);

    auto listener = std::make_shared<SelectListener>();
    Reactor reactor(listener);

    int calls = 0;

    reactor.Add(pipefd[0], Reactor::READ,
        [&](int fd, Reactor::Mode)
        {
            char c;
            read(fd, &c, 1);
            PrintValidity(c, 'A', "got A");

            ++calls;
            reactor.Stop();
        });

    write(pipefd[1], "A", 1);
    write(pipefd[1], "B", 1);

    reactor.Run();

    PrintValidity(calls, 1, "only one call");

    close(pipefd[0]);
    close(pipefd[1]);

    std::cout << std::endl;
}

template <typename T>
static void PrintValidity(T result, T expected_result, std::string test_name)
{
    if (result == expected_result)
    {
        std::cout << COLOR_GREEN "GOOD" COLOR_RESET " for: " << test_name << std::endl;
    }
    else
    {
        std::cout << COLOR_RED "BAD" COLOR_RESET " for: " << test_name << std::endl;
        std::cout << "Expected: " << expected_result << ", Got: " << result << std::endl;
    }
}

/**************************************
Exercise: 	final project - Async Injection 
Date:		18/12/2025
Developer:	baruch haimson
Reviewer: 	
Status:		
**************************************/
#include <iostream>            // std::cout
#include <atomic>              // std::atomic
#include <thread>              // std::this_thread

#include "asyncinjection.hpp"
#include "handleton.hpp"

using namespace ilrd;

static int counter = 0;

int main()
{    
    new AsyncInjection(
        []() -> bool
        {
            ++counter;
            std::cout << "Tick " << counter << std::endl;
            
            return counter >= 5;
        },
        std::chrono::milliseconds(500)
    );
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    if (counter == 5)
    {
        std::cout << "PASSED - counter reached 5" << std::endl;
    }
    else
    {
        std::cout << "FAILED - counter is " << counter << std::endl;
    }

    return 0;
}

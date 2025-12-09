/*
g++ -std=gnu++11 -fPIC -Iinclude -c src/handleton.cpp -o src/handleton.o

g++ -shared -o libhandleton.so src/handleton.o

g++ -std=gnu++11 -pthread -Iinclude src/logger.cpp test/logger_test.cpp \
    -L. -lhandleton -Wl,-rpath,'$ORIGIN' -o logger_test

LD_LIBRARY_PATH=. ./logger_test

*/

#include "logger.hpp"

int main()
{
    ilrd::Logger& logger = ilrd::Handleton<ilrd::Logger>::GetInstance();

    logger.SetLevel(ilrd::Logger::INFO);
    logger.Log("Program started", ilrd::Logger::INFO,__FILE__, __LINE__);

    logger.SetLevel(ilrd::Logger::DEBUG);
    logger.Log("baruchi", ilrd::Logger::ERROR,__FILE__, __LINE__);

    logger.SetLevel(ilrd::Logger::ERROR);
    logger.Log("dont meed to see this messsege!!!!", ilrd::Logger::INFO);

    logger.SetLevel(ilrd::Logger::DEBUG);
    logger.Log("ben", ilrd::Logger::ERROR,__FILE__, __LINE__);
}

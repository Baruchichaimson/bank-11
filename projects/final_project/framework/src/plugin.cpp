/*
compile with handleton:

g++ -std=gnu++11 -fPIC -Iinclude \
    -c src/plugin.cpp -o src/plugin.o

g++ -shared -o libb.so src/plugin.o \
    -L. -lhandleton
*/

#include <iostream>
#include "singleton.hpp"

extern template class ilrd::Singleton<int>;
extern "C" void Foo()
{
    int* a_plugin = ilrd::Singleton<int>::getInstance();
    std::cout << "Plugin: Singleton<int> instance at " << a_plugin << std::endl;
}

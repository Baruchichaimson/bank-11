/*
build with:

g++ -std=gnu++11 -fPIC -Iinclude \
    -c src/handleton.cpp -o src/handleton.o

g++ -shared -o libhandleton.so src/handleton.o
*/

#define __HANDLETON__

#include "singleton.hpp"
#include "logger.hpp"

namespace ilrd
{
    // Explicit instantiations so the symbols live in libhandleton.so
    template class Singleton<int>;
    template class Singleton<Logger>;
}

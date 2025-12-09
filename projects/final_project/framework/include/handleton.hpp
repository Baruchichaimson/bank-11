#ifndef ILRD_HANDLETON_HPP
#define ILRD_HANDLETON_HPP

#include "singleton.hpp"

namespace ilrd
{
// Thin wrapper that exposes GetInstance() as a reference-returning API.
template <typename T>
class Handleton
{
public:
    static T& GetInstance()
    {
        return *Singleton<T>::getInstance();
    }

    Handleton() = delete;
    Handleton(const Handleton&) = delete;
    Handleton& operator=(const Handleton&) = delete;
};

} // namespace ilrd

#endif // ILRD_HANDLETON_HPP

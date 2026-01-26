#ifndef __ILRD_FACTORY__
#define __ILRD_FACTORY__

#include <unordered_map> // std::unorderd_map
#include <functional> // std::function
#include <memory> // std::shared_ptr
#include <stdexcept> // std::out_of_range

#include "logger.hpp"

namespace ilrd
{
    
template<typename BASE, typename KEY, typename... ARGS>     
class Factory
{
public:
    using BASE_PTR = std::shared_ptr<BASE>;
    using CTOR = std::function<BASE_PTR(ARGS...)>;
    
    Factory() = default;
    ~Factory() noexcept = default;
    void Add(KEY key, CTOR ctor) noexcept;
    BASE_PTR Create(KEY key, ARGS... args); // throw std::out_of_range

    Factory(const Factory& other) = delete;
    Factory& operator=(const Factory& other) = delete; 
    Factory(const Factory&& other) = delete;
    Factory&& operator=(const Factory&& other) = delete;    

private:

    std::unordered_map<KEY, CTOR> m_map;

}; // class Factory

template<typename BASE, typename KEY, typename... ARGS>     
void Factory< BASE,  KEY, ARGS...>::Add(KEY key, CTOR ctor) noexcept
{
    LOG_DEBUG("Factory::Add entered");
    m_map[key] = ctor;
    LOG_DEBUG("Factory::Add exit");
}

template<typename BASE, typename KEY, typename... ARGS>     
typename Factory< BASE,  KEY, ARGS...>::BASE_PTR Factory< BASE,  KEY, ARGS...>::Create(KEY key, ARGS... args)
{
    LOG_DEBUG("Factory::Create entered");
    auto iter = m_map.find(key);
    if (iter == m_map.end())
    {
        throw std::out_of_range("Factory: key not found");
    }
    auto result = iter->second(std::forward<ARGS>(args)...);
    LOG_DEBUG("Factory::Create exit");
    return result;
}


} // ilrd

#endif // __ILRD_FACTORY__

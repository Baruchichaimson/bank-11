/**************************************
Exercise: 	cpp_intro - rc_string
Date:		26/10/2025
Developer:	Baruch Haimson
Reviewer: 	
Status:		
**************************************/
#include <cstddef>
#include <stdexcept> // for std::out_of_range
#include <new>
#include "rcstring.hpp"

namespace ilrd
{
    RCString::RCString(const char* str) : m_rc(NULL)
    {
        const char* s = str ? str : "";
        
        size_t len = std::strlen(s);
        size_t total_size = offsetof(RCImp, m_str) + len + 1;

        void* mem = operator new(total_size);
        RCImp* rc = static_cast<RCImp*>(mem);
        rc->m_count = 1;
        std::strcpy(rc->m_str, s);
        m_rc = rc;
    }

    RCString::RCString(const RCString& other) : m_rc(other.m_rc)
    {
        ++m_rc->m_count;
    }

    RCString& RCString::operator=(const RCString& other)
    {
        if (this != &other)
        {
            if (--m_rc->m_count == 0)
            {
                operator delete(m_rc);
            }

            m_rc = other.m_rc;
            ++m_rc->m_count;
        }
        return *this;
    }

    RCString::~RCString() noexcept
    {
        if (--m_rc->m_count == 0)
        {
            operator delete(m_rc);
        }
    }

    char RCString::operator[](size_t idx) const
    {
        if (idx >= Length())
        {
            throw std::out_of_range("RCString index out of range");
        }
        return m_rc->m_str[idx];
    }

    char& RCString::operator[](size_t idx)
    {
        if (idx >= Length())
        {
            throw std::out_of_range("RCString index out of range");
        }

        if (m_rc->m_count > 1)
        {
            /* Copy-on-write: יוצרים עותק חדש */
            size_t len = Length();
            size_t total_size = offsetof(RCImp, m_str) + len + 1;
            void* mem = operator new(total_size);
            RCImp* new_rc = static_cast<RCImp*>(mem);
            new_rc->m_count = 1;
            std::strcpy(new_rc->m_str, m_rc->m_str);

            --m_rc->m_count;
            m_rc = new_rc;
        }

        return m_rc->m_str[idx];
    }

    std::ostream& operator<<(std::ostream& os, const RCString& other)
    {
        os << other.ToCStr();
        return os;
    }

    std::istream& operator>>(std::istream& is, RCString& other)
    {
        char buffer[1024];
        is >> buffer;
        other = RCString(buffer);
        return is;
    }
}

/************************************
 * Exercise:    Simple String
 * Date:        23/10/2025
 * Developer:   Baruch Haimson
 * Reviewer:    Maria
 * Status:      In Progress
 ************************************/

#include <cstring> // strlen

#include "simple_string.hpp"

namespace ilrd
{
    String::String(const char* str) : m_str(Init(str))
    { 
    }

    String::String(const String& other) : m_str(Init(other.m_str))
    {
    }

    String& String::operator=(const String& other)
    {
        char* temp = Init(other.m_str);

        delete[] m_str;
        m_str = temp;
        
        return *this;
    }

    String::~String()
    {
        delete[] m_str;
        m_str = NULL;
    }

    size_t String::Length() const
    {
        return strlen(m_str);
    }

    char* String::Cstr() const
    {
        return m_str;
    }

    bool operator==(const String& lhs, const String& rhs)
    {
        return 0 == strcmp(lhs.Cstr(), rhs.Cstr());
    }

    bool operator>(const String& lhs, const String& rhs)
    {
        return 0 < strcmp(lhs.Cstr(), rhs.Cstr());
    }

    bool operator<(const String& lhs, const String& rhs)
    {
        return 0 > strcmp(lhs.Cstr(), rhs.Cstr());
    }

    std::ostream& operator<<(std::ostream& os, const String& other)
    {
        os << other.Cstr();

        return os;
    }

    std::istream& operator>>(std::istream& is, String& other)
    {
        char buffer[80] = {0};
        
        is >> buffer;

        other = buffer;
        
        return is;
    }
    
    char* String::Init(const char* str)
    {
        char* temp = new char[strlen(str) + 1];
        strcpy(temp, str);
        return temp;
    }
}

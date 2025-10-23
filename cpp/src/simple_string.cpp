#include <cstring> // strlen

#include "simple_string.hpp"

using namespace ilrd;
using namespace std;


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

char* String::Init(const char* str)
{
    char* temp = NULL;
    size_t len = 0;

    if (str == NULL)
    {
        temp = new char[1];
        temp[0] = '\0';
        return temp;
    }
    else
    {
        len = strlen(str);
        temp = new char[len + 1];
        strcpy(temp, str);
        return temp;
    }
}

bool ilrd::operator==(const String& lhs, const String& rhs)
{
    return strcmp(lhs.Cstr(), rhs.Cstr()) == 0;
}

bool ilrd::operator>(const String& lhs, const String& rhs)
{
    return strcmp(lhs.Cstr(), rhs.Cstr()) > 0;
}

bool ilrd::operator<(const String& lhs, const String& rhs)
{
    return strcmp(lhs.Cstr(), rhs.Cstr()) < 0;
}

ostream& ilrd::operator<<(ostream& os, const String& other)
{
    os << other.Cstr();

    return os;
}

istream& ilrd::operator>>(istream& is, String& other)
{
    string temp;
    is >> temp;

    other = String(temp.c_str());

    return is;
}

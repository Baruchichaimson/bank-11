/**************************************
Exercise: 	cpp_intro - rc_string
Date:		26/10/2025
Developer:	Baruch Haimson
Reviewer: 	
Status:		
**************************************/

#ifndef __ILRD_RCSTRING__
#define __ILRD_RCSTRING__

#if __cplusplus < 201103L
#  define noexcept throw()
#endif

#include <iostream> //std::istream 
#include <cstring>


namespace ilrd
{
    class RCString
    {
    public:	
        RCString(const char* str = ""); //non-explicit on purpose
        RCString(const RCString& other);
        RCString& operator=(const RCString& other);
        ~RCString() noexcept;
        inline char* ToCStr() const noexcept;
        inline size_t Length() const noexcept; 
        char operator[](size_t idx) const; 
        char& operator[](size_t idx);
    private:
        struct RCImp
        {
            size_t m_count;
            char m_str[1];
        } *m_rc;
    }; //RCString class

    inline bool operator==(const RCString& lhs, const RCString& rhs);
    inline bool operator!=(const RCString& lhs, const RCString& rhs);
    inline bool operator>(const RCString& lhs, const RCString& rhs);
    inline bool operator<(const RCString& lhs, const RCString& rhs);

    std::ostream& operator<<(std::ostream& os, const RCString& other);
    std::istream& operator>>(std::istream& is, RCString& other);	

    char* RCString::ToCStr() const noexcept
    {
        return m_rc->m_str;
    }

    size_t RCString::Length() const noexcept
    {
        return std::strlen(m_rc->m_str);
    }

    bool operator==(const RCString& lhs, const RCString& rhs)
    {
        return std::strcmp(lhs.ToCStr(), rhs.ToCStr()) == 0;
    }

    bool operator!=(const RCString& lhs, const RCString& rhs)
    {
        return !(lhs == rhs);
    }

    bool operator>(const RCString& lhs, const RCString& rhs)
    {
        return std::strcmp(lhs.ToCStr(), rhs.ToCStr()) > 0;
    }

    bool operator<(const RCString& lhs, const RCString& rhs)
    {
        return std::strcmp(lhs.ToCStr(), rhs.ToCStr()) < 0;
    }

} //ilrd
#endif //__ILRD_RCSTRING__
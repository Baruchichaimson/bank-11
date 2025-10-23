/************************************
 * Exercise:    Simple String
 * Date:        23/10/2025
 * Developer:   Baruch Haimson
 * Reviewer:    Maria
 * Status:      In Progress
 ************************************/

#ifndef __ILRD_STRING__
#define __ILRD_STRING__

#include <iostream> // istream

namespace ilrd
{
    class String
    {
    public:
        String(const char* str = ""); // non-explicit on purpose
        String(const String& other);
        String& operator=(const String& other);
        ~String();
        size_t Length() const;
        char* Cstr() const;
        
    private:
        char* m_str;
        char* Init(const char* str);
        // helper functions
    };// String class
        
    bool operator==(const String& lhs, const String& rhs);
    bool operator>(const String& lhs, const String& rhs);
    bool operator<(const String& lhs, const String& rhs);
    std::ostream& operator<<(std::ostream& os, const String& other); //advanced
    std::istream& operator>>(std::istream& is, String& other); //advanced
}// ilrd

#endif //__ILRD_STRING__
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void itoa10(int value, char* str)
{
	char tmp;
    char* p = str;
    char* start;
    char* q;
    char* r;
    int isNegative = 0;

    if (value == 0)
    {
        *p = '0';
        p++;
        *p = '\0';
        return;
    }

    if (value < 0)
    {
        isNegative = 1;
        value = -value;
    }

    start = p;
    while (value > 0)
    {
        *p = '0' + (value % 10);
        p++;
        value /= 10;
    }

    if (isNegative) 
    {	
    	*p = '-';
    	p++;
    	*p = '\0';
    }
	q = start;
	r = p - 1;
    while (q < r)
    {
        tmp = *q;
        *q = *r; 
        *r = tmp;
        ++q;
        --r;
    }
}

int atoi10(const char* str)
{
    int result = 0; 
    int sign = 1;
    while (*str == ' ') 
    {
    	++str;
    }
    if (*str == '-') 
    { 
    	sign = -1; 
    	++str; 
    }
    else if (*str == '+') 
    {
    	++str;
	}
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        ++str;
    }
    return sign * result;
}

void ItoaBase(int value, char* str, int base)
{
	const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* p = str;
    int remainder;
    char* start;
    char* q;
    char* r;
    char tmp;
    int is_negative = 0;
    
    if (base < 2 || base > 36)
    { 
    	*str = '\0';
     	return; 
    }
    if (value == 0) 
    { 
    	*p++ = '0';
    	*p = '\0'; 
    	return; 
    }
    if (value < 0 && base == 10) 
    { 	
    	is_negative = 1;
    	value = -value; 
    }

    start = p;
    while (value > 0)
    {
    	remainder = value % base;
        if (remainder < 0 || remainder >= 36)
        {
            fprintf(stderr, "ERROR: invalid index to digits[%d]\n", remainder);
            *str = '\0';
            return;
        }
        *p++ = digits[remainder];
        value /= base;
    }

    if (is_negative) 
    {
    	*p++ = '-';
    }
    *p = '\0';
    
    q = start;
    r = p - 1;
    while (q < r)
    {
        tmp = *q;
        *q = *r; 
        *r = tmp;
        ++q;
        --r;
    }
}

int AtoiBase(const char *str, int base) 
{
    int digit = 0;
    int result = 0;
    int sign = 1;
    if (base == 10) 
    {
        return atoi10(str);
    }
    
    if (base < 2 || base > 36)
    {
        return 0;
    }
    
    while (isspace(*str))
    {
        str++;
    }
    
    if (*str == '-') 
    {
        sign = -1; 
        str++;
    }
    else if (*str == '+') 
    {
        str++;
    }
    
    while (*str) 
    {
        
        if (isdigit(*str)) 
            digit = *str - '0';
        else if (isupper(*str)) 
            digit = *str - 'A' + 10;
        else if (islower(*str)) 
            digit = *str - 'a' + 10;
        else 
            break;
            
        if (digit >= base) 
        {
            break;
        }
        
        result = result * base + digit;
        str++;
    }
    
    return result * sign;
}

void PrintUniqueChars(const char* a, const char* b, const char* c)
{
	size_t i = 0;
    int in_a[256] = {0}, in_b[256] = {0}, in_c[256] = {0};

    while (*a)
    {
    	in_a[(unsigned char)*a] = 1;
    	++a;
    }
    while (*b)
    {
    	 in_b[(unsigned char)*b] = 1;
    	 ++b;
    }
    while (*c)
    {
    	 in_c[(unsigned char)*c] = 1;
		 ++c;
    }

    for (i = 0; i < 256; ++i)
    {
        if (in_a[i] && in_b[i] && !in_c[i])
            putchar(i);
    }
    putchar('\n');
}
int IsLittleEndian()
{
    int x = 1;
    return *((char*)&x) == 1;
}



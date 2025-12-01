/**************************************
Exercise: bit array
Date: 27/11/2025
Developer: Baruch Haimson
Reviewer: tal
Status:	
**************************************/
#ifndef __BIT_ARRAY_HPP__
#define __BIT_ARRAY_HPP__

#include <stdlib.h>

class BitArray
{
    private:
        class BitProxy;
    public:
        explicit BitArray(size_t bit_arr = 0);
        BitProxy operator[](size_t index);
        bool operator[](size_t index) const;
        
    private:
        class BitProxy
        {
            public:
                BitProxy(BitArray& bit_arr,size_t index);
                BitProxy& operator=(bool val);
                BitProxy& operator=(BitProxy& other);
                operator bool() const;

            private:
            BitArray& m_arr;
            size_t m_index;

            BitProxy& operator&(BitProxy& bit) = delete;
            BitProxy& operator&(const BitProxy& bit) = delete;
        };
        size_t m_bit_arr;
};

#endif

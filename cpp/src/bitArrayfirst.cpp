/**************************************
Exercise: bit array
Date: 27/11/2025
Developer: Baruch Haimson
Reviewer: tal
Status:	waiting
**************************************/
#include "bitArrayfirst.hpp"

BitArray::BitArray(size_t bit_arr) : m_bit_arr(bit_arr)
{
}

BitArray::BitProxy BitArray::operator[](size_t index)
{
    return BitProxy(*this,index);
}

bool BitArray::operator[](size_t index) const
{
    return (m_bit_arr & ((size_t)1 << index)) != 0; // bitproxy not receive const bitArray& this is why i am check myself the bit.
}

BitArray::BitProxy::BitProxy(BitArray& bit_arr,size_t index) :m_arr(bit_arr), m_index(index)
{
}
BitArray::BitProxy& BitArray::BitProxy::operator=(bool val)
{
    if (val)
    {
        m_arr.m_bit_arr |= ((size_t)1 << m_index);  
    }
    else
    {
        m_arr.m_bit_arr &= ~((size_t)1 << m_index);
    }  

    return *this;
}
BitArray::BitProxy& BitArray::BitProxy::operator=(BitProxy& other)
{
    /* *this = static_cast<bool>(other);*/       //one way
    *this = other;                               //second way
    /* *this = (size_t)1 << other.m_index; */    //thrid way

    return *this;
}
BitArray::BitProxy::operator bool() const
{
    return (m_arr.m_bit_arr & ((size_t)1 << m_index)) != 0;
}
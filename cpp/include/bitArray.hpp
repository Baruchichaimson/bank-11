/**************************************
Exercise: bit array
Date: 27/11/2025
Developer: Baruch Haimson
Reviewer: tal
Status:	waiting
**************************************/

#ifndef __ILRD_BIT_ARRAY__
#define __ILRD_BIT_ARRAY__

#include <climits> // CHAR_BIT
#include <algorithm>

namespace ilrd
{

    const size_t SIZE_OF_SIZE_T = CHAR_BIT * sizeof(size_t);

    template <std::size_t SIZE>
    class BitArray
    {
    private:
        class BitProxy;
    public:
        explicit BitArray() noexcept;
        bool operator[](std::size_t idx) const;       
        BitProxy operator[](std::size_t idx);        
        
        BitArray& operator|=(const BitArray& other) noexcept;  
        BitArray& operator&=(const BitArray& other) noexcept; 
        BitArray& operator^=(const BitArray& other) noexcept; 
        bool operator==(const BitArray& other) const noexcept; 
        bool operator!=(const BitArray& other) const noexcept;
        BitArray& operator<<=(size_t n); 
        BitArray& operator>>=(size_t n); 
        void Set(std::size_t idx, bool val);
        void SetAll(bool val) noexcept;
        bool Get(std::size_t idx) const;
        void Flip(std::size_t idx);
        void Flip() noexcept;
        std::size_t Count() const noexcept;
        std::string ToString() const;
        
    private:
        static constexpr std::size_t num_bits = (SIZE - 1) / SIZE_OF_SIZE_T + 1;
        std::size_t m_bits[num_bits];

        class BitProxy
        {
        public:
            BitProxy(BitArray& bit_arr, std::size_t idx) noexcept;
        
            BitProxy& operator=(bool val);               //  b[i] = x
            BitProxy& operator=(const BitProxy& other);  // for b[i] = b[j]
            BitProxy& Flip();                       // l-value use
            
            operator bool() const noexcept; // x = b[i]

        private:
            BitArray& m_arr;
            std::size_t m_idx;
            
            // redundant
            BitProxy& operator&(BitProxy&) = delete;             // forbid & operator
            BitProxy& operator&(const BitProxy&) const = delete; // forbid & operator
        };// BitProxy

    }; // BitArray class

    template <std::size_t SIZE>
    BitArray<SIZE>::BitArray() noexcept
    {
        std::fill(std::begin(m_bits), std::end(m_bits), 0);
    }

    template <std::size_t SIZE>
    bool BitArray<SIZE>::operator[](std::size_t idx) const 
    {
        return (m_bits[idx / SIZE_OF_SIZE_T] & ((size_t)1 << (idx % SIZE_OF_SIZE_T))) != 0;
    }

    template <std::size_t SIZE>
    typename BitArray<SIZE>::BitProxy BitArray<SIZE>::operator[](std::size_t idx) 
    {
        return BitProxy(*this,idx);
    }      
        
    template <std::size_t SIZE>
    BitArray<SIZE>& BitArray<SIZE>::operator|=(const BitArray& other) noexcept
    {
        std::transform(std::begin(m_bits), std::end(m_bits), std::begin(other.m_bits), std::begin(m_bits), std::bit_or<size_t>());
        return *this;
    }

    template <std::size_t SIZE>
    BitArray<SIZE>& BitArray<SIZE>::operator&=(const BitArray& other) noexcept
    {
        std::transform(std::begin(m_bits), std::end(m_bits), std::begin(other.m_bits), std::begin(m_bits), std::bit_and<size_t>());
        return *this; 
    }

    template <std::size_t SIZE>
    BitArray<SIZE>& BitArray<SIZE>::operator^=(const BitArray& other) noexcept
    {
        std::transform(std::begin(m_bits), std::end(m_bits), std::begin(other.m_bits), std::begin(m_bits), std::bit_xor<size_t>());
        return *this;
    }

    template <std::size_t SIZE>
    bool BitArray<SIZE>::operator==(const BitArray& other) const noexcept
    {
        return std::equal(std::begin(m_bits), std::end(m_bits), std::begin(other.m_bits));
    }

    template <std::size_t SIZE>
    bool BitArray<SIZE>::operator!=(const BitArray& other) const noexcept
    {
        return !(*this == other);
    }

    template <std::size_t SIZE>
    BitArray<SIZE>& BitArray<SIZE>::operator<<=(size_t n) 
    {
        std::transform(std::begin(m_bits), std::end(m_bits), std::begin(m_bits), [n](size_t val) { return val << n; });
        return *this;
    }

    template <std::size_t SIZE>
    BitArray<SIZE>& BitArray<SIZE>::operator>>=(size_t n) 
    {

    std::transform(std::begin(m_bits), std::end(m_bits), std::begin(m_bits), [n](size_t val) { return val >> n; });
        return *this;
    }

    template <std::size_t SIZE>
    void BitArray<SIZE>::Set(std::size_t idx, bool val) 
    {
        if (val)
        {
            m_bits[idx / SIZE_OF_SIZE_T] |= ((size_t)1 << (idx % SIZE_OF_SIZE_T));  
        }
        else
        {
            m_bits[idx / SIZE_OF_SIZE_T] &= ~((size_t)1 << (idx % SIZE_OF_SIZE_T));
        }
    }

    template <std::size_t SIZE>
    void BitArray<SIZE>::SetAll(bool val) noexcept
    {
        std::fill(std::begin(m_bits), std::end(m_bits), val ? ~0 : 0);
    }

    template <std::size_t SIZE>
    bool BitArray<SIZE>::Get(std::size_t idx) const 
    {
        return (m_bits[idx / SIZE_OF_SIZE_T] & ((size_t)1 << (idx % SIZE_OF_SIZE_T))) != 0;
    }

    template <std::size_t SIZE>
    void BitArray<SIZE>::Flip(std::size_t idx) 
    {
        m_bits[idx / SIZE_OF_SIZE_T] ^= ((size_t)1 << (idx % SIZE_OF_SIZE_T));
    }

    template <std::size_t SIZE>
    void BitArray<SIZE>::Flip() noexcept
    {
        std::transform(std::begin(m_bits), std::end(m_bits) ,std::begin(m_bits), std::bit_not<size_t>());
    }

    template <std::size_t SIZE>
    std::size_t BitArray<SIZE>::Count() const noexcept
    {
        std::size_t count = 0;

        std::for_each(std::begin(m_bits), std::end(m_bits), [&count](std::size_t val) {
            while (val)
            {
                val &= (val - 1); 
                ++count;
            }
        });

        return count;
    }

    template <std::size_t SIZE>
    std::string BitArray<SIZE>::ToString() const 
    {
        std::string result(SIZE, '0');
        std::generate(result.begin(), result.end(), [this]()  
        {
            static size_t i = 0; 
            return this->Get(i++) ? '1' : '0';
        });

        return result;
    }



    template <std::size_t SIZE>
    BitArray<SIZE>::BitProxy::BitProxy(BitArray& bit_arr, std::size_t idx) noexcept :m_arr(bit_arr), m_idx(idx)
    {
    }
    
    template <std::size_t SIZE>
    typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator=(bool val) 
    {
        if (val)
        {
            m_arr.m_bits[m_idx / SIZE_OF_SIZE_T] |= ((size_t)1 << (m_idx % SIZE_OF_SIZE_T));  
        }
        else
        {
            m_arr.m_bits[m_idx / SIZE_OF_SIZE_T] &= ~((size_t)1 << (m_idx % SIZE_OF_SIZE_T));
        }  

        return *this;
    }             

    template <std::size_t SIZE>
    typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator=(const BitProxy& other) 
    {
        if (this != &other)
        {
            m_arr.m_bits[m_idx / SIZE_OF_SIZE_T] &= ~((size_t)1 << (m_idx % SIZE_OF_SIZE_T));
            m_arr.m_bits[m_idx / SIZE_OF_SIZE_T] |= (other.m_arr.m_bits[other.m_idx / SIZE_OF_SIZE_T] & ((size_t)1 << (other.m_idx % SIZE_OF_SIZE_T)));
        }
        return *this;
    }

    template <std::size_t SIZE>
    typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::Flip() 
    {
        m_arr.m_bits[m_idx / SIZE_OF_SIZE_T] ^= ((size_t)1 << (m_idx % SIZE_OF_SIZE_T));
        return *this;
    }                   

    template <std::size_t SIZE>
    BitArray<SIZE>::BitProxy::operator bool() const noexcept
    {
        return (m_arr.m_bits[m_idx / SIZE_OF_SIZE_T] & ((size_t)1 << (m_idx % SIZE_OF_SIZE_T))) != 0;
    }
} //ilrd

#endif //__ILRD_BIT_ARRAY__
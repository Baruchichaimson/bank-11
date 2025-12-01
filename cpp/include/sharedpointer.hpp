/**************************************
Exercise: 	shared pointer
Date:		25/11/2025
Developer:	Baruch Haimson
Reviewer: 	
Status:		
**************************************/

#ifndef __SHARED_PTR__
#define __SHARED_PTR__

#include <cstddef>   // size_t

namespace ilrd
{
    template <typename T>
    class SharedPtr
    {
    public:
        explicit SharedPtr(T* ptr = NULL): m_ptr(ptr), m_ref_count(new size_t(1))
        {}

        SharedPtr(const SharedPtr& other): m_ptr(other.m_ptr), m_ref_count(other.m_ref_count)
        {
            if (m_ref_count) 
            {
                ++(*m_ref_count);
            }
        }

        template <typename U> // upcasting
        SharedPtr(const SharedPtr<U>& other): m_ptr(other.m_ptr), m_ref_count(other.m_ref_count)
        {
            if (m_ref_count) 
            {
                ++(*m_ref_count);
            }
        }

        ~SharedPtr()
        {
            Release();
        }

        SharedPtr& operator=(SharedPtr other) 
        {
            if (this != &other)
            {
                Release();  
                m_ptr = other.m_ptr;
                m_ref_count = other.m_ref_count;
                ++(*m_ref_count);
            }
            return *this;
        }

        T& operator*() const
        {
            return *m_ptr;
        }

        T* operator->() const
        {
            return m_ptr;
        }

        T* Get() const
        {
            return m_ptr;
        }

        size_t UseCount() const
        {
            return m_ref_count ? *m_ref_count : 0;
        }

    private:

        template <typename U>
        friend class SharedPtr;

        void Release()
        {
            if (m_ref_count)
            {
                --(*m_ref_count);

                if (*m_ref_count == 0)
                {
                    delete m_ptr;
                    delete m_ref_count;
                }
            }
        }

        T* m_ptr;
        size_t* m_ref_count;
    };
}
#endif // __SHARED_PTR__

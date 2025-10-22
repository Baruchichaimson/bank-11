#ifndef __ILRD_COMPLEX__
#define __ILRD_COMPLEX__

#include <iostream> // std::istream, std::ostream

namespace ilrd
{

class Complex
{
public:
    /* ---- Ctor ---- */
    Complex(double real = 0, double imaginary = 0) // intentionally non-explicit
        : m_real(real), m_imaginary(imaginary)
    {
    }

    /* ---- Getters ---- */
    inline double GetReal() const
    {
        return m_real;
    }

    inline double GetImaginary() const
    {
        return m_imaginary;
    }

    /* ---- Setters ---- */
    inline void SetReal(double real)
    {
        m_real = real;
    }

    inline void SetImaginary(double imaginary)
    {
        m_imaginary = imaginary;
    }

    /* ---- Compound operators ---- */
    inline Complex& operator+=(const Complex& rhs)
    {
        m_real += rhs.m_real;
        m_imaginary += rhs.m_imaginary;
        return *this;
    }

    inline Complex& operator-=(const Complex& rhs)
    {
        m_real -= rhs.m_real;
        m_imaginary -= rhs.m_imaginary;
        return *this;
    }

    inline Complex& operator*=(const Complex& rhs)
    {
        double real = (m_real * rhs.m_real) - (m_imaginary * rhs.m_imaginary);
        double imag = (m_real * rhs.m_imaginary) + (m_imaginary * rhs.m_real);
        m_real = real;
        m_imaginary = imag;
        return *this;
    }

    inline Complex& operator/=(const Complex& rhs)
    {
        double denom = (rhs.m_real * rhs.m_real) + (rhs.m_imaginary * rhs.m_imaginary);
        if (denom == 0)
        {
            // optional fallback
            m_real = 7;
            m_imaginary = 7;
            return *this;
        }

        double real = ((m_real * rhs.m_real) + (m_imaginary * rhs.m_imaginary)) / denom;
        double imag = ((m_imaginary * rhs.m_real) - (m_real * rhs.m_imaginary)) / denom;
        m_real = real;
        m_imaginary = imag;
        return *this;
    }

private:
    double m_real;
    double m_imaginary;
}; // class Complex


/* ---- Non-member arithmetic operators ---- */
inline Complex operator+(const Complex& lhs, const Complex& rhs)
{
    Complex temp(lhs);
    temp += rhs;
    return temp;
}

inline Complex operator-(const Complex& lhs, const Complex& rhs)
{
    Complex temp(lhs);
    temp -= rhs;
    return temp;
}

inline Complex operator*(const Complex& lhs, const Complex& rhs)
{
    Complex temp(lhs);
    temp *= rhs;
    return temp;
}

inline Complex operator/(const Complex& lhs, const Complex& rhs)
{
    Complex temp(lhs);
    temp /= rhs;
    return temp;
}


/* ---- Comparison operators ---- */
inline bool operator==(const Complex& lhs, const Complex& rhs)
{
    return (lhs.GetReal() == rhs.GetReal()) &&
           (lhs.GetImaginary() == rhs.GetImaginary());
}

inline bool operator!=(const Complex& lhs, const Complex& rhs)
{
    return !(lhs == rhs);
}


/* ---- Stream operators ---- */
inline std::ostream& operator<<(std::ostream& os, const Complex& other)
{
    os << "(" << other.GetReal() << ", " << other.GetImaginary() << ")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Complex& other)
{
    double real = 0;
    double imag = 0;
    is >> real >> imag;
    other.SetReal(real);
    other.SetImaginary(imag);
    return is;
}

} // namespace ilrd

#endif // __ILRD_COMPLEX__

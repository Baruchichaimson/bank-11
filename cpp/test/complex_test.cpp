/************************************
 * Exercise:    Simple String
 * Date:        23/10/2025
 * Developer:   Baruch Haimson
 * Reviewer:    meir
 * Status:      Approved
 ************************************/

#include <iostream>
#include <ctime>

#include "complex.hpp"

int main()
{
    ilrd::Complex c;
    ilrd::Complex res;
    ilrd::Complex a(3, 4);
    ilrd::Complex b(1, 2);

    std::cout << "c = " << c << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    std::cout << "a + b = " << (a + b) << std::endl;
    std::cout << "a - b = " << (a - b) << std::endl;
    std::cout << "a * b = " << (a * b) << std::endl;
    std::cout << "a / b = " << (a / b) << std::endl;

    //std::cout << std::boolalpha;
    std::cout << "a == b? " << (a == b) << std::endl;
    std::cout << "a != b? " << (a != b) << std::endl;

    clock_t start = clock();
    for (int i = 0; i < 100000; ++i)
    {
        res = a + b; // by value
    }
    clock_t end = clock();

    std::cout << "Time (by value): " << double(end - start) / CLOCKS_PER_SEC << " sec\n";

    return 0;
}


/**************************************
Exercise: bit array
Date: 27/11/2025
Developer: Baruch Haimson
Reviewer: tal
Status:	waiting
**************************************/
#include <iostream>

#include "bitArrayfirst.hpp"

int main()
{
    BitArray b1;
    BitArray b2;

    std::cout << "Test 1: Set and get bit\n";
    b1[0] = true;
    b1[1] = false;
    b1[2] = true;

    std::cout << "b1[0] = " << b1[0] << " (expected 1)\n";
    std::cout << "b1[1] = " << b1[1] << " (expected 0)\n";
    std::cout << "b1[2] = " << b1[2] << " (expected 1)\n\n";

    b1[3] = true;
    b1[4] = true;
    b1[5] = false;

    std::cout << "b1[3] = " << b1[3] << " (expected 1)\n";
    std::cout << "b1[4] = " << b1[4] << " (expected 1)\n";
    std::cout << "b1[5] = " << b1[5] << " (expected 0)\n\n";



    b2[0] = false;
    b2[1] = true;
    b2[2] = false;

    std::cout << "Before copy:\n";
    std::cout << "b2[0] = " << b2[0] << "\n";
    std::cout << "b2[1] = " << b2[1] << "\n";
    std::cout << "b2[2] = " << b2[2] << "\n";

    b2[0] = b1[0];
    b2[1] = b1[1];
    b2[2] = b1[2];

    std::cout << "After copy from b1:\n";
    std::cout << "b2[0] = " << b2[0] << " (expected 1)\n";
    std::cout << "b2[1] = " << b2[1] << " (expected 0)\n";
    std::cout << "b2[2] = " << b2[2] << " (expected 1)\n\n";

    std::cout << "Test 4: if statement\n";
    if (b1[0])
        std::cout << "b1[0] is true\n"; 
    else
        std::cout << "b1[0] is false\n";

    if (b1[1])
        std::cout << "b1[1] is true\n";
    else
        std::cout << "b1[1] is false\n";

    b1[5] = true;
    std::cout << "\nAfter setting b1[5] = true:\n";
    std::cout << "b1[5] = " << b1[5] << " (expected 1)\n";

    std::cout << "\nUnchanged bits:\n";
    std::cout << "b1[6] = " << b1[6] << " (expected 0)\n";
    std::cout << "b2[3] = " << b2[3] << " (expected 0)\n";

    return 0;
}

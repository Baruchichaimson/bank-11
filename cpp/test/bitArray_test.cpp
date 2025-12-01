/**************************************
Exercise: bit array
Date: 27/11/2025
Developer: Baruch Haimson
Reviewer: tal
Status:	waiting
**************************************/

#include <iostream>
#include "bitArray.hpp"

using namespace ilrd;

int main()
{
    try
    {
        constexpr size_t SIZE = 16;
        BitArray<SIZE> b1;
        BitArray<SIZE> b2;

        std::cout << "Initial b1: " << b1.ToString() << "\n";
        std::cout << "Initial b2: " << b2.ToString() << "\n";

        // ---- Set and Get ----
        b1.Set(0, true);
        b1.Set(3, true);
        b1.Set(15, true);
        std::cout << "b1 after setting bits 0, 3, 15: " << b1.ToString() << "\n";

        if (!b1.Get(1) && b1.Get(3) && b1.Get(15))
        {
            std::cout << "Get/Set test passed\n";
        }

        // ---- BitProxy assignment ----
        b2[0] = b1[3];
        b2[15] = b1[0];
        std::cout << "b2 after proxy assignment: " << b2.ToString() << "\n";

        // ---- Operators |=, &=, ^= ----
        BitArray<SIZE> b3;
        b3.SetAll(true);
        std::cout << "b3 (all 1): " << b3.ToString() << "\n";

        b1 |= b2;
        std::cout << "b1 |= b2: " << b1.ToString() << "\n";

        b1 &= b3;
        std::cout << "b1 &= b3: " << b1.ToString() << "\n";

        b1 ^= b2;
        std::cout << "b1 ^= b2: " << b1.ToString() << "\n";

        // ---- Flip ----
        b1.Flip(0);
        std::cout << "b1 after flipping bit 0: " << b1.ToString() << "\n";

        b1.Flip();
        std::cout << "b1 after flipping all bits: " << b1.ToString() << "\n";

        // ---- Shift operators ----
        BitArray<SIZE> b4;
        b4.Set(0, true);
        b4.Set(1, true);
        b4.Set(2, true);
        std::cout << "b4 before shift: " << b4.ToString() << "\n";
        b4 <<= 2;
        std::cout << "b4 <<= 2: " << b4.ToString() << "\n";
        b4 >>= 1;
        std::cout << "b4 >>= 1: " << b4.ToString() << "\n";

        // ---- Equality / Inequality ----
        BitArray<SIZE> b5;
        b5.SetAll(false);
        BitArray<SIZE> b6;
        b6.SetAll(false);

        if (b5 == b6)
        {
            std::cout << "Equality test passed\n";
        }

        b6.Set(0, true);
        if (b5 != b6)
        {
            std::cout << "Inequality test passed\n";
        }

        // ---- Count ----
        b1.SetAll(false);
        b1.Set(0, true);
        b1.Set(5, true);
        b1.Set(15, true);
        std::cout << "b1: " << b1.ToString() << "\n";
        std::cout << "b1 Count: " << b1.Count() << " (should be 3)\n";

        // ---- Negation / bool conversion ----
        if (!b1[1] && b1[0])
        {
            std::cout << "Bool conversion / negation test passed\n";
        }

        // ---- Edge cases ----
        BitArray<1> b_small;
        b_small.Set(0, true);
        std::cout << "Single-bit array: " << b_small.ToString() << "\n";
        std::cout << "Count: " << b_small.Count() << "\n";

        std::cout << "All tests completed successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    return 0;
}

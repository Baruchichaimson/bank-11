#include <iostream>
#include "sharedpointer.hpp"

using namespace ilrd;

struct Base 
{
    Base() : x(10) {}
    virtual ~Base() {}
    int x;
    void Print() { std::cout << "Base::Print(), x=" << x << "\n"; }
};

struct Derived : public Base 
{
    Derived() : Base(), y(20) {}
    int y;
    void Print() { std::cout << "Derived::Print(), x=" << x << ", y=" << y << "\n"; }
};

void TestCtor()
{
    SharedPtr<int> sp(new int(5));
    std::cout << "TestCtor: " << (sp.UseCount() == 1) << "\n";
}

void TestCopy()
{
    SharedPtr<int> sp1(new int(10));
    SharedPtr<int> sp2 = sp1;

    std::cout << "TestCopy1: " << (sp1.UseCount() == 2) << "\n";
    std::cout << "TestCopy2: " << (sp2.UseCount() == 2) << "\n";
}

void TestAssignment()
{
    SharedPtr<int> sp1(new int(7));
    SharedPtr<int> sp2(new int(99));

    sp2 = sp1;

    std::cout << "TestAssignment1: " << (sp1.UseCount() == 2) << "\n";
    std::cout << "TestAssignment2: " << (sp2.UseCount() == 2) << "\n";
}

void TestUpcasting()
{
    SharedPtr<Derived> d(new Derived());
    SharedPtr<Base> b = d; // upcast

    std::cout << "TestUpcasting1: " << (d.UseCount() == 2) << "\n";
    std::cout << "TestUpcasting2: " << (b.UseCount() == 2) << "\n";

    b->Print(); // virtual - will still call Base::Print (unless you make Print virtual)
}

void TestDestructor()
{
    SharedPtr<int>* spA = new SharedPtr<int>(new int(11));
    SharedPtr<int> spB = *spA;

    std::cout << "Before delete spA: " << (spB.UseCount() == 2) << "\n";

    delete spA;

    std::cout << "After delete spA: " << (spB.UseCount() == 1) << "\n";
}

void TestOperators()
{
    SharedPtr<Derived> sp(new Derived());

    std::cout << "operator*: " << ((*sp).x == 10) << "\n";
    std::cout << "operator->: " << (sp->y == 20) << "\n";
}

int main()
{
    TestCtor();
    TestCopy();
    TestAssignment();
    TestUpcasting();
    TestDestructor();
    TestOperators();

    std::cout << "\nAll tests finished.\n";
    return 0;
}

#include <iostream>

#include "factory.hpp"

using namespace ilrd;

class Shape
{
public:
    virtual void Print() const = 0; 
    virtual ~Shape() = default;
};

class Circle : public Shape
{
public:
    Circle(double r) : radius(r) 
    {}

    void Print() const override
    {
        std::cout << "Circle with radius: " << radius << "\n";
    }
private:
    double radius;
};

class Square : public Shape
{
public:
    Square(double s) : side(s) 
    {}

    void Print() const override
    {
        std::cout << "Square with side: " << side << "\n";
    }
private:
    double side;
};

int main()
{
    Factory<Shape, std::string, double> f;

    f.Add("circle", [](double r){ return std::make_shared<Circle>(r); });
    f.Add("square", [](double s){ return std::make_shared<Square>(s); });

    auto c = f.Create("circle", 5.0);
    auto sq = f.Create("square", 10.0);

    c->Print();   
    sq->Print();  

    try
    {
        auto unknown = f.Create("triangle", 3.0);
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "Exception caught: " << e.what() << "\n";
    }

    return 0;
}
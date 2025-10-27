#include <iostream>
#include "rcstring.hpp"
using namespace ilrd;

int main()
{
	RCString s1("Hello");
	RCString s2 = s1;
    RCString s3 = "baruch";

	std::cout << "Before change: s1=" << s1 << " s2=" << s2 << " s3=" << s3 << std::endl;

	s2[0] = 'J';
    s3[4] = 'B';

	std::cout << "After change:  s1=" << s1 << " s2=" << s2 << " s3=" << s3 << std::endl;

	std::cout << "Equal? " << (s1 == s2 ? "Yes" : "No") << std::endl;
}

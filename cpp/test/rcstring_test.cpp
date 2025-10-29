#include <iostream>

#include "rcstring.hpp"

int main()
{
	try
	{
		ilrd::RCString s1("Hello");
		ilrd::RCString s2 = s1;
		ilrd::RCString s3 = "baruch";

		std::cout << "Before change: s1=" << s1 << " s2=" << s2 << " s3=" << s3 << std::endl;

		s2[0] = 'J';
		s3[4] = 'B';   

		std::cout << "After change:  s1=" << s1 << " s2=" << s2 << " s3=" << s3 << std::endl;

		std::cout << "Equal? " << (s1 == s2 ? "Yes" : "No") << std::endl;

		std::cout << "Trying invalid index access..." << std::endl;
		char ch = s3[100]; 
		std::cout << "Character at 100: " << ch << std::endl;
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Memory allocation failed: " << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "General exception: " << e.what() << std::endl;
	}

	std::cout << "Program finished safely." << std::endl;
	return 0;
		
}

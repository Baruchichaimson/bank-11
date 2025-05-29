#include <stdio.h>

int flip(int num)
{
	int digit;
	int result = 0;
	while(num)
	{
		digit = num % 10;
		result = result * 10 + digit;
		num /= 10;
	}
	return result; 
}

int main()
{
	printf("%d\n" , flip(43210));
	return 0;
}


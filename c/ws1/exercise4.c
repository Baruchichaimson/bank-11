#include <stdio.h>

int pow10(int n)
{
	int result = 1;
	for (int i = 0; i < n; i++)
	{
		result *= 10;
	}
	return result;
}
int main()
{
	printf("%d\n", pow10(5));
	return 0;
}

#include <stdio.h>

typedef void (*Print)(int);

void print(int val) { printf("The value is : %d\n",val); }
struct print_me 
{
    int value;
    Print print;
};

int main()
{
	size_t i;
	struct print_me arr[10];
	
	for(i = 0; i < 10;i++)
	{
		arr[i].value = i + 6;
		arr[i].print = print;
	}
	for(i = 0; i < 10;i++)
	{
		arr[i].print(arr[i].value);
	}
	return 0;
}

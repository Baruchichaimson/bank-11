#include <stdio.h>
#include <string.h>
#include <stddef.h>

void TF(int num)
{
	int i;
	for(i = 1; i <= num; i++)
	{	
		if (0 == i % 3 && 0 == i % 5)
		{
			printf("TF\n");
		}
		else if (0 == i % 3)
		{
			printf("T\n");
		}
		else if (0 == i % 5)
		{
			printf("F\n");
		}
		else if(i % 3 != 0 && i % 5 != 0)
		{
			printf("%d\n", i);
		}
	}
}

void reverses(char *str) {
    int size = strlen(str);
    char *start = str;
    char *end = str + size - 1;
    char temp;

    while (start < end) {
     
        if (*start >= 'A' && *start <= 'Z') {
            *start = *start + ('a' -'A');
        }
        if (*end >= 'A' && *end <= 'Z') {
            *end = *end + ('a' -'A');
        }

        temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}
void TF_Test()
{
	TF(15);
	
}

void reverses_Test()
{
	char str[] = "jhfdshGJHWQwh87984#$#@$gjhewg";      
	
	printf("Input:  %s\n", str);
	
	reverses(str);
	
	printf("Output: %s\n", str);
}

int main()
{
	printf("Test TF_Test:\n");
	TF_Test();
	
	printf("Test ReverseStrAndLowCaseTest:\n");
	reverses_Test();
	
	return 0;
}



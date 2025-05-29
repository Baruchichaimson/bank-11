#include <stdio.h>

double pow10(int n)
{
    double result = 1.0;
    int abs_n = n < 0 ? -n : n;

    for (int i = 0; i < abs_n; i++)
    {
        result *= 10.0;
    }

    if (n < 0)
    {
        return 1.0 / result;
    }

    return result;
}

int main()
{
    printf("%f\n", pow10(5));  
    printf("%f\n", pow10(-3));  
    return 0;
}

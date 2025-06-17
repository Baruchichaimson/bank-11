#include <stdio.h>

#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) (MAX2(MAX2(a, b), c))


int main() 
{
    int x = 7, y = 20, z = 15;
    printf("MAX2(x, y): %d\n", MAX2(x, y));        /* => 20 */
    printf("MAX3(x, y, z): %d\n", MAX3(x, y, z));  /* => 20 */
    return 0;
}

#include <stdio.h>

#define SIZEOF_VAR(x) ((char*)(&x + 1) - (char*)(&x))
/* casting to char* because we want to count how much byte wh have ((char*) = 1 byte) */

int main() 
{
    int a;
    double b;
    char c;

    printf("SIZEOF_VAR(a) = %ld\n", SIZEOF_VAR(a)); /* => 4 */
    printf("SIZEOF_VAR(b) = %ld\n", SIZEOF_VAR(b)); /* => 8 */ 
    printf("SIZEOF_VAR(c) = %ld\n", SIZEOF_VAR(c)); /* => 1 */

    return 0;
}

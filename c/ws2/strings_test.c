#include "strings.h"

int main()
{
    const char *Nullable = NULL;
    const char *s1 = "baruch haimson";
    const char *s2 = "baruch haimson";
    const char *s3 = "baruch iaimson";
    const char *s4 = "baruch aimson";

    assert(StrLen("baruch haimson") == 14);
    assert(StrLen("") == 0);
    assert(StrLen(Nullable) == 0);

    printf("All StrLen tests passed successfully!\n");

    assert(StrCmp(s1, s2) == 0);
    assert(StrCmp(s1, s3) == -1); 
    assert(StrCmp(s1, s4) == 1);   

    printf("All StrCmp tests passed successfully!\n");

    return 0;
}

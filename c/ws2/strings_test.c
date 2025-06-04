#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "strings.h"

int main()
{
    const char *Nullable = NULL;
    const char *s1 = "baruch haimson";
    const char *s2 = "baruch haimson";
    const char *s3 = "baruch iaimson";
    const char *s4 = "baruch aimson";
    char buffer1[10];
    char buffer2[10];
    char *dup;
    char cat_buffer[20] = "hello ";
    char ncat_buffer[20] = "foo";
    char input[] = "hello,world,this,is,a,test";
    char *token;

    /* StrLen */
    assert(StrLen("baruch haimson") == 14);
    assert(StrLen("") == 0);
    assert(StrLen(Nullable) == 0);
    printf("All StrLen tests passed successfully!\n");

    /* StrCmp */
    assert(StrCmp(s1, s2) == 0);
    assert(StrCmp(s1, s3) == -1);
    assert(StrCmp(s1, s4) == 1);
    printf("All StrCmp tests passed successfully!\n");

    /* StrnCmp */
    assert(StrnCmp("abcde", "abcdf", 4) == 0);
    assert(StrnCmp("abcde", "abcdz", 5) == -1);
    assert(StrnCmp("abcdz", "abcde", 5) == 1);
    printf("All StrnCmp tests passed successfully!\n");

    /* StrCaseCmp */
    assert(StrCaseCmp("HeLLo", "hello") == 0);
    assert(StrCaseCmp("abc", "ABC") == 0);
    assert(StrCaseCmp("abc", "ABD") == -1);
    assert(StrCaseCmp("abc", "ABB") == 1);
    printf("All StrCaseCmp tests passed successfully!\n");

    /* StrChr */
    assert(StrChr("hello", 'e') == &"hello"[1]);
    assert(StrChr("hello", 'x') == NULL);
    assert(StrChr("test", '\0') == &"test"[4]);
    printf("All StrChr tests passed successfully!\n");

    /* StrnCpy */
    assert(strcmp(StrnCpy(buffer1, "baruch", 6), "baruch") == 0);
    StrnCpy(buffer2, "bar", 5);
    assert(strcmp(buffer2, "bar") == 0);
    printf("All StrnCpy tests passed successfully!\n");

    /* StrDup */
    dup = StrDup("hello");
    assert(strcmp(dup, "hello") == 0);
    free(dup);
    printf("All StrDup tests passed successfully!\n");

    /* StrCat */
    assert(strcmp(StrCat(cat_buffer, "world"), "hello world") == 0);
    printf("All StrCat tests passed successfully!\n");

    /* StrnCat */
    assert(strcmp(StrnCat(ncat_buffer, "barbaz", 3), "foobar") == 0);
    printf("All my_strncat tests passed successfully!\n");

    /* StrStr */
    assert(strcmp(StrStr("hello world", "world"), "world") == 0);
    assert(StrStr("hello", "abc") == NULL);
    assert(StrStr("abcabcabc", "cab") != NULL);
    printf("All my_strstr tests passed successfully!\n");

    /* StrSpn */
    assert(StrSpn("abcde123", "abcde") == 5);
    assert(StrSpn("123abc", "abc") == 0);
    assert(StrSpn("abcabcabc", "abc") == 9);
    printf("All my_strspn tests passed successfully!\n");

    /* my_strtok */
    token = StrTok(input, ",");
    assert(strcmp(token, "hello") == 0);
    token = StrTok(NULL, ",");
    assert(strcmp(token, "world") == 0);
    token = StrTok(NULL, ",");
    assert(strcmp(token, "this") == 0);
    token = StrTok(NULL, ",");
    assert(strcmp(token, "is") == 0);
    token = StrTok(NULL, ",");
    assert(strcmp(token, "a") == 0);
    token = StrTok(NULL, ",");
    assert(strcmp(token, "test") == 0);
    token = StrTok(NULL, ",");
    assert(token == NULL);
    printf("All my_strtok tests passed successfully!\n");

    printf("All string function tests passed!\n");
    return 0;
}

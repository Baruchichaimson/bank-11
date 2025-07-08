#include <stdio.h>
#include <string.h>
#include "cbuff.h"

/***************************
 Exercise:  cbuff
 Date: 	    07/07/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    Approved
***************************/

void TestCreate()
{
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    cb = CBuffCreate(capacity);
    
    if (cb)
    {
        printf("TestCreate: PASS\n");
    }
    else
    {
        printf("TestCreate: FAIL\n");
    }
    
    CBuffDestroy(cb);
}

void TestDestroy()
{
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    cb = CBuffCreate(capacity);
    
    CBuffDestroy(cb);
    
    printf("TestDestroy: PASS\n");
}

void TestWrite()
{
    const char* src = "abba";
    size_t bytes = strlen(src);
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    cb = CBuffCreate(capacity);
    
    ssize_t amount = CBuffWrite(cb, src, bytes);
    
    if (amount == (ssize_t)bytes)
    {
        printf("TestWrite: PASS, wrote %zd bytes\n", amount);
    }
    else
    {
        printf("TestWrite: FAIL, wrote %zd bytes\n", amount);
    }

    CBuffDestroy(cb);
}

void TestRead()
{
    const char* src = "abba";
    size_t bytes = strlen(src);
    char dst[10] = {0};
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    
    cb = CBuffCreate(capacity);
    CBuffWrite(cb, src, bytes);
    ssize_t amount = CBuffRead(cb, dst, bytes);
    
    if (amount == (ssize_t)bytes && memcmp(dst, src, bytes) == 0)
    {
        printf("TestRead: PASS, read %zd bytes: %s\n", amount, dst);
    }
    else
    {
        printf("TestRead: FAIL, read %zd bytes: %s\n", amount, dst);
    }

    CBuffDestroy(cb);
}

void TestIsEmpty()
{
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    cb = CBuffCreate(capacity);

    if (CBuffIsEmpty(cb))
    {
        printf("TestIsEmpty: PASS (initially empty)\n");
    }
    else
    {
        printf("TestIsEmpty: FAIL (should be empty)\n");
    }

    const char *src = "x";
    CBuffWrite(cb, src, 1);

    if (!CBuffIsEmpty(cb))
    {
        printf("TestIsEmpty after write: PASS\n");
    }
    else
    {
        printf("TestIsEmpty after write: FAIL\n");
    }

    CBuffDestroy(cb);
}

void TestFreeSpace()
{
    const char *src = "abcd";
    size_t bytes = strlen(src);
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    cb = CBuffCreate(capacity);

    size_t free_before = CBuffFreeSpace(cb);
    CBuffWrite(cb, src, bytes);
    size_t free_after = CBuffFreeSpace(cb);

    if (free_before == capacity && free_after == capacity - bytes)
    {
        printf("TestFreeSpace: PASS\n");
    }
    else
    {
        printf("TestFreeSpace: FAIL\n");
    }

    CBuffDestroy(cb);
}

void TestSize()
{
    const char *src = "abcd";
    size_t bytes = strlen(src);
    cbuff_t* cb = NULL;
    size_t capacity = 1024;
    cb = CBuffCreate(capacity);

    CBuffWrite(cb, src, bytes);
    
    size_t size = CBuffSize(cb);

    if (size == bytes)
    {
        printf("TestSize: PASS and the size is: %zu\n", size);
    }
    else
    {
        printf("TestSize: FAIL, got %zu\n", size);
    }

    CBuffDestroy(cb);
}

int main()
{
    TestCreate();
    TestDestroy();
    TestWrite();
    TestRead();
    TestIsEmpty();
    TestFreeSpace();
    TestSize();

    printf("\nAll tests completed.\n");
    return 0;
}


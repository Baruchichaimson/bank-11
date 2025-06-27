#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"

/***************************
 Exercise:  stack
 Date: 	    27/06/25
 Developer: Baruch Haimson
 Reviewer:  Bar
 Status:    
***************************/

void TestCreate()
{
    printf("=== Test: StackCreate ===\n");

    stack_t *s1 = StackCreate(0, sizeof(int));
    printf("Test: Create with capacity 0 ( Expected: != NULL | Actual: %s\n", s1 ? "!= NULL)" : "NULL)");

    stack_t *s2 = StackCreate(1, sizeof(int));
    printf("Test: Create with capacity 1 ( Expected: != NULL | Actual: %s\n", s2 ? "!= NULL)" : "NULL)");

    stack_t *s3 = StackCreate(1000, sizeof(double));
    printf("Test: Create with capacity 1000 ( Expected: != NULL | Actual: %s\n", s3 ? "!= NULL)" : "NULL)");

    StackDestroy(s1);
    StackDestroy(s2);
    StackDestroy(s3);
}

void TestDestroy()
{
    printf("\n=== Test: StackDestroy ===\n");
/*======================================test 1===================================*/
    stack_t *s2 = StackCreate(3, sizeof(int));
    int a = 42, b = 84;

    StackPush(s2, &a);
    StackPush(s2, &b);

    printf("Test: Before destroy | Expected size: 2 | Actual: %lu\n", StackSize(s2));
    printf("Test: Top element before destroy | Expected: 84 | Actual: %d\n", *(int *)StackPeek(s2));

    printf("Test: Destroy after usage | Expected: destroyed\n");
    StackDestroy(s2);
    printf("destroyed Success\n");
/*======================================test 2===================================*/
    stack_t *s1 = StackCreate(2, sizeof(int));
    printf("Test: Destroy after empty creation | Expected: destroyed\n");
    StackDestroy(s1);
    printf("destroyed Success\n");
    
/*======================================test 3===================================*/
    stack_t *s3 = StackCreate(1, sizeof(double));
    printf("Test: Destroy single-element stack | Expected: destroyed\n");
    StackDestroy(s3);
    printf("destroyed Success\n");
}


void TestPush()
{
    printf("\n=== Test: StackPush ===\n");

    stack_t *s = StackCreate(3, sizeof(int));
    int a = 1, b = 2, c = 3;

    StackPush(s, &a);
    printf("Test: Push 1 | Expected top: 1 | Actual: %d\n", *(int *)StackPeek(s));

    StackPush(s, &b);
    printf("Test: Push 2 | Expected top: 2 | Actual: %d\n", *(int *)StackPeek(s));

    StackPush(s, &c);
    printf("Test: Push 3 | Expected top: 3 | Actual: %d\n", *(int *)StackPeek(s));

    StackDestroy(s);
}

void TestPop()
{
    printf("\n=== Test: StackPop ===\n");

    stack_t *s = StackCreate(3, sizeof(int));
    int a = 10, b = 20, c = 30;

    StackPush(s, &a);
    StackPush(s, &b);
    StackPush(s, &c);

    StackPop(s);
    printf("Test: Pop once | Expected top: 20 | Actual: %d\n", *(int *)StackPeek(s));

    StackPop(s);
    printf("Test: Pop twice | Expected top: 10 | Actual: %d\n", *(int *)StackPeek(s));

    StackPop(s);
    printf("Test: Pop third | Expected top: pop will be empty");
	printf("Size after pop everything: %lu\n", StackSize(s)); 
    StackDestroy(s);
}

void TestPeek()
{
    printf("\n=== Test: StackPeek ===\n");

    stack_t *s = StackCreate(2, sizeof(int));
    int a = 111, b = 222;

    StackPush(s, &a);
    printf("Test: Peek after push 111 | Expected: 111 | Actual: %d\n", *(int *)StackPeek(s));

    StackPush(s, &b);
    printf("Test: Peek after push 222 | Expected: 222 | Actual: %d\n", *(int *)StackPeek(s));

    StackPop(s);
    printf("Test: Peek after pop | Expected: 111 | Actual: %d\n", *(int *)StackPeek(s));

    StackDestroy(s);
}

void TestSize()
{
    printf("\n=== Test: StackSize ===\n");

    stack_t *s = StackCreate(3, sizeof(int));
    printf("Test: Size after create | Expected: 0 | Actual: %lu\n", StackSize(s));

    int x = 7;
    StackPush(s, &x);
    printf("Test: Size after 1 push | Expected: 1 | Actual: %lu\n", StackSize(s));

    StackPush(s, &x);
    StackPush(s, &x);
    printf("Test: Size after 3 pushes | Expected: 3 | Actual: %lu\n", StackSize(s));

    StackPop(s);
    printf("Test: Size after 1 pop | Expected: 2 | Actual: %lu\n", StackSize(s));

    StackDestroy(s);
}

void TestIsEmpty()
{
    printf("\n=== Test: StackIsEmpty ===\n");

    stack_t *s = StackCreate(2, sizeof(int));
    printf("Test: IsEmpty after create | Expected: 1 | Actual: %d\n", StackIsEmpty(s));

    int x = 100;
    StackPush(s, &x);
    printf("Test: IsEmpty after 1 push | Expected: 0 | Actual: %d\n", StackIsEmpty(s));

    StackPop(s);
    printf("Test: IsEmpty after pop | Expected: 1 | Actual: %d\n", StackIsEmpty(s));

    StackDestroy(s);
}

void TestCapacity()
{
    printf("\n=== Test: StackCapacity ===\n");

    stack_t *s1 = StackCreate(0, sizeof(int));
    printf("Test: Capacity 0 | Expected: 0 | Actual: %lu\n", StackCapacity(s1));
    StackDestroy(s1);

    stack_t *s2 = StackCreate(1, sizeof(int));
    printf("Test: Capacity 1 | Expected: 1 | Actual: %lu\n", StackCapacity(s2));
    StackDestroy(s2);

    stack_t *s3 = StackCreate(1024, sizeof(int));
    printf("Test: Capacity 1024 | Expected: 1024 | Actual: %lu\n", StackCapacity(s3));
    StackDestroy(s3);
}

int main()
{
    TestCreate();
    TestDestroy();
    TestPush();
    TestPop();
    TestPeek();
    TestSize();
    TestIsEmpty();
    TestCapacity();

    return 0;
}


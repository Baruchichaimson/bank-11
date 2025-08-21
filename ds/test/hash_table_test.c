#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "hash_table.h"
#include "sll.h"

/******************** Test Helpers ********************/


static size_t HashFuncInt(const void* data)
{
    return (*(int*)data);
}


static int IsMatchInt(const void* data1, void* data2)
{
    return (*(int*)data1 == *(int*)data2);
}


static int PrintAction(void* table_data, void* param)
{
    (void)param;
    printf("%d ", *(int*)table_data);
    return 0;
}


/******************** Unit Tests ********************/
void TestHashTableCreate()
{
    hash_table_t* table = NULL;
    printf("--- TestHashTableCreate ---\n");
    table = HashTableCreate(HashFuncInt, 3, IsMatchInt);
    assert(table);
    assert(HashTableIsEmpty(table) == 1);
    assert(HashTableSize(table) == 0);
    HashTableDestroy(table);


    table = HashTableCreate(HashFuncInt, 10, IsMatchInt);
    assert(table);
    HashTableDestroy(table);
    printf("PASS\n");
}


void TestHashTableDestroy()
{
    hash_table_t* table = NULL;
    int a=1;
    printf("--- TestHashTableDestroy ---\n");
    table = HashTableCreate(HashFuncInt, 3, IsMatchInt);
    
    HashTableInsert(table,&a);
    HashTableDestroy(table);
    printf("PASS (no crash)\n");
}


void TestHashTableInsert()
{
    hash_table_t* table = NULL;
    int a=1,b=6,c=11;
    printf("--- TestHashTableInsert ---\n");
    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    
    assert(HashTableInsert(table,&a)==0);
    assert(HashTableInsert(table,&b)==0);
    assert(HashTableInsert(table,&c)==0);
    assert(HashTableSize(table)==3);
    HashTableDestroy(table);
    printf("PASS\n");
    }

void TestHashTableRemove()
{
    hash_table_t* table = NULL;
    int a=1,b=2,c=3;
    printf("--- TestHashTableRemove ---\n");
    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    
    HashTableInsert(table,&a);
    HashTableInsert(table,&b);
    HashTableInsert(table,&c);
    assert(HashTableSize(table)==3);
    HashTableRemove(table,&b);
    assert(HashTableFind(table,&b)==NULL);
    assert(HashTableSize(table)==2);
    HashTableRemove(table,&a);
    HashTableRemove(table,&c);
    assert(HashTableIsEmpty(table)==1);
    HashTableDestroy(table);
    printf("PASS\n");
}


void TestHashTableFind()
{
    hash_table_t* table = NULL;
    int a=10,b=20,c=30;
    printf("--- TestHashTableFind ---\n");
    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    
    HashTableInsert(table,&a);
    HashTableInsert(table,&b);
    assert(*(int*)HashTableFind(table,&a)==10);
    assert(*(int*)HashTableFind(table,&b)==20);
    assert(HashTableFind(table,&c)==NULL);
    HashTableDestroy(table);
    printf("PASS\n");
}


void TestHashTableSize()
{
    hash_table_t* table = NULL;
    int a=1,b=2;
    printf("--- TestHashTableSize ---\n");
    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    
    assert(HashTableSize(table)==0);
    HashTableInsert(table,&a);
    assert(HashTableSize(table)==1);
    HashTableInsert(table,&b);
    assert(HashTableSize(table)==2);
    HashTableRemove(table,&a);
    assert(HashTableSize(table)==1);
    HashTableDestroy(table);
    printf("PASS\n");
}


void TestHashTableIsEmpty()
{
    hash_table_t* table = NULL;
    int a=1;
    printf("--- TestHashTableIsEmpty ---\n");
    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    
    assert(HashTableIsEmpty(table)==1);
    HashTableInsert(table,&a);
    assert(HashTableIsEmpty(table)==0);
    HashTableRemove(table,&a);
    assert(HashTableIsEmpty(table)==1);
    HashTableDestroy(table);
    printf("PASS\n");
}


void TestHashTableForEach()
{
    hash_table_t* table = NULL;
    int a=1,b=2,c=3;
    printf("--- TestHashTableForEach ---\n");
    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    
    HashTableInsert(table,&a);
    HashTableInsert(table,&b);
    HashTableInsert(table,&c);
    printf("Elements: ");
    HashTableForEach(table, PrintAction, NULL);
    printf("\n");
    HashTableDestroy(table);
    printf("PASS\n");
}


int main()
{
    TestHashTableCreate();
    TestHashTableDestroy();
    TestHashTableInsert();
    TestHashTableRemove();
    TestHashTableFind();
    TestHashTableSize();
    TestHashTableIsEmpty();
    TestHashTableForEach();

    return 0;
}
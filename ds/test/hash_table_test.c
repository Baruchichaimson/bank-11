#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"
#include "sll.h"

#define DICT_FILE ("/usr/share/dict/words")
#define MAX_WORD_LEN (256)

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

/******************** String Helpers for Dictionary ********************/

static size_t HashFuncString(const void* data)
{
    const char* str = (const char*)data;
    size_t hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

static int IsMatchString(const void* data1, void* data2)
{
    return strcmp((const char*)data1, (const char*)data2) == 0;
}

static int FreeString(void* table_data, void* param)
{
    (void)param;
    free(table_data);
    return 1;
}

/******************** Dictionary Functions ********************/

hash_table_t* LoadDictionary(const char* filename)
{
    char buffer[MAX_WORD_LEN];
    hash_table_t* table = NULL;
    FILE* file = fopen(filename, "r");
    char* word = NULL;
    if (!file) 
    {
        return NULL;
    }

    table = HashTableCreate(HashFuncString, 10000, IsMatchString);
    if (!table) 
    {
        fclose(file);
        return NULL;
    }

    
    while (fgets(buffer, MAX_WORD_LEN, file))
    {
        buffer[strcspn(buffer, "\n")] = 0; 

        word = malloc(strlen(buffer) + 1);  
        if (word)
        {
            strcpy(word, buffer);
        }
    
        HashTableInsert(table, word);
    }

    fclose(file);
    return table;
}

void SpellChecker(hash_table_t* dict)
{
    char word[MAX_WORD_LEN];
    printf("Enter a word: ");
    if (scanf("%255s", word) != 1) return;

    if (HashTableFind(dict, word))
    {
        printf("Correct\n");
    }
    else
    {
        printf("Incorrect\n");
    }
}

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

/*================== Advance ======================*/

void TestHashTableFindMoveToFront()
{
    hash_table_t* table = NULL;
    sll_t* bucket = NULL;
    sll_iter_t iter = { NULL };
    int a = 1;
    int b = 6;

    printf("--- TestHashTableFindMoveToFront ---\n");

    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);

    HashTableInsert(table, &a);
    HashTableInsert(table, &b);

    assert(HashTableFindMoveFront(table, &a) == &a);
    assert(HashTableFindMoveFront(table, &b) == &b);

    bucket = table->buckets[table->hash_func(&a) % table->capacity];
    iter = SLLBegin(bucket);
    assert(SLLGetData(iter) == &b); 

    HashTableDestroy(table);
    printf("PASS\n");
}


void TestHashTableLoadFactor()
{
    hash_table_t* table = NULL;
    int a = 1;
    int b = 2;
    int c = 3;
    printf("--- TestHashTableLoadFactor ---\n");

    table = HashTableCreate(HashFuncInt, 5, IsMatchInt);
    assert(table);

    assert(HashTableLoadFactor(table) == 0.0);

    HashTableInsert(table, &a);
    HashTableInsert(table, &b);
    HashTableInsert(table, &c);

    assert(HashTableLoadFactor(table) == 0.6);

    HashTableDestroy(table);
    printf("PASS\n");
}

int main()
{
    hash_table_t* dict = NULL;
    TestHashTableCreate();
    TestHashTableDestroy();
    TestHashTableInsert();
    TestHashTableRemove();
    TestHashTableFind();
    TestHashTableSize();
    TestHashTableIsEmpty();
    TestHashTableForEach();

    printf("\n--- Dictionary / Spell Checker Test ---\n");
    dict = LoadDictionary(DICT_FILE);
    if (!dict)
    {
        printf("Failed to load dictionary.\n");
        return 1;
    }
    SpellChecker(dict);

    HashTableForEach(dict, FreeString, NULL);
    HashTableDestroy(dict);

    TestHashTableFindMoveToFront();
    TestHashTableLoadFactor();
    return 0;
}
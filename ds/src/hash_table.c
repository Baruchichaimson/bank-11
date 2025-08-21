#include <stdlib.h>
#include <assert.h>

#include "hash_table.h"

hash_table_t* HashTableCreate(hash_func_t hash_func, size_t capacity, is_match_func_t is_match)
{
    size_t i = 0;
    hash_table_t* table = NULL;


    assert(hash_func);
    assert(is_match);


    table = (hash_table_t*)malloc(sizeof(hash_table_t));
    if (!table)
    {
        return NULL;
    }


    table->capacity = capacity;
    table->hash_func = hash_func;
    table->is_match = is_match;


    table->buckets = (sll_t**)malloc(sizeof(sll_t*) * capacity);
    if (!table->buckets)
    {
        free(table);
        return NULL;
    }


    for (i = 0; i < capacity; ++i)
    {
        table->buckets[i] = SLLCreate();
        if (!table->buckets[i])
        {
            while (i > 0)
            {
                --i;
                SLLDestroy(table->buckets[i]);
            }
            
            free(table->buckets);
            free(table);
            return NULL;
        }
    }
    return table;
}

void HashTableDestroy(hash_table_t* table)
{
    size_t i = 0;
    if (!table)
    {
        return;
    }

    for (i = 0; i < table->capacity; ++i)
    {
        SLLDestroy(table->buckets[i]);
    }

    free(table->buckets);
    free(table);
}


int HashTableInsert(hash_table_t* table, void* data)
{
    size_t index = 0;
    sll_t* bucket = NULL;


    assert(table);
    assert(data);


    index = table->hash_func(data) % table->capacity;
    bucket = table->buckets[index];


    if (!SLLInsert(SLLEnd(bucket), data))
    {
        return 1;
    }


    return 0; 
}

void HashTableRemove(hash_table_t* table, const void* data)
{
    size_t index = 0;
    sll_iter_t iter;
    sll_t* bucket = NULL;


    assert(table);
    assert(data);


    index = table->hash_func(data) % table->capacity;
    bucket = table->buckets[index];


    iter = SLLFind(SLLBegin(bucket), SLLEnd(bucket), table->is_match, (void*)data);


    if (!SLLIsEqual(iter, SLLEnd(bucket)))
    {
        SLLRemove(iter);
    }
}


void* HashTableFind(const hash_table_t* table, const void* data)
{
    size_t index = 0;
    sll_iter_t iter;
    sll_t* bucket = NULL;


    assert(table);
    assert(data);


    index = table->hash_func(data) % table->capacity;
    bucket = table->buckets[index];


    iter = SLLFind(SLLBegin(bucket), SLLEnd(bucket), table->is_match, (void*)data);


    return (SLLIsEqual(iter, SLLEnd(bucket))) ? NULL : SLLGetData(iter);
}

size_t HashTableSize(const hash_table_t* table)
{
    size_t i = 0, size = 0;
    assert(table);


    for (i = 0; i < table->capacity; ++i)
    {
        size += SLLCount(table->buckets[i]);
    }


    return size;
}


int HashTableIsEmpty(const hash_table_t* table)
{
    size_t i = 0;
    assert(table);


    for (i = 0; i < table->capacity; ++i)
    {
        if (!SLLIsEmpty(table->buckets[i]))
        {
            return 0; 
        }
    }
    return 1;
}

int HashTableForEach(hash_table_t* table, action_func_t action_func, void* param)
{
    size_t i = 0;
    assert(table);
    assert(action_func);


    for (i = 0; i < table->capacity; ++i)
    {
        if (SLLForEach(SLLBegin(table->buckets[i]), SLLEnd(table->buckets[i]), action_func, param))
        {
            return 1; 
        }
    }
    return 0;
}
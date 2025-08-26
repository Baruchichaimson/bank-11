/************************************
 * Exercise: hash table 
 * Date: 21/08/25
 * Developer: Baruch Haimson
 * Reviewer: daniel
 * Status: Aprroved
 ************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "hash_table.h" /* API */

hash_table_t* HashTableCreate(hash_func_t hash_func, size_t capacity, is_match_func_t is_match)
{
    size_t i = 0;
    hash_table_t* table = NULL;

    assert(hash_func);
    assert(is_match);

    if (capacity == 0)
    {
        return NULL;
    }

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
    if (table->buckets)
    {
        for (i = 0; i < table->capacity; ++i)
        {
            SLLDestroy(table->buckets[i]);
        }

        free(table->buckets);
    }
    free(table);
}


int HashTableInsert(hash_table_t* table, void* data)
{
    size_t index = 0;
    sll_iter_t found = { NULL };
    sll_t* bucket = NULL;

    assert(table);
    assert(data);

    index = table->hash_func(data) % table->capacity;
    bucket = table->buckets[index];

    assert(bucket);

    found = SLLFind(SLLBegin(bucket), SLLEnd(bucket), table->is_match, data);

    assert(SLLIsEqual(found, SLLEnd(bucket)));

    if (!SLLInsert(SLLEnd(bucket), data))
    {
        return 1;
    }

    return 0; 
}

void HashTableRemove(hash_table_t* table, const void* data)
{
    size_t index = 0;
    sll_iter_t iter = { NULL };
    sll_t* bucket = NULL;


    assert(table);
    assert(data);


    index = table->hash_func(data) % table->capacity;
    bucket = table->buckets[index];

    assert(bucket);

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

    assert(bucket);

    iter = SLLFind(SLLBegin(bucket), SLLEnd(bucket), table->is_match, (void*)data);


    return (SLLIsEqual(iter, SLLEnd(bucket))) ? NULL : SLLGetData(iter);
}

size_t HashTableSize(const hash_table_t* table)
{
    size_t i = 0;
    size_t size = 0;

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

/*================== Advance ======================*/

void* HashTableFindMoveFront(hash_table_t* table, const void* data)
{
    size_t index;
    sll_iter_t iter;
    sll_t* bucket;
    void* found_data;

    assert(table);
    assert(data);

    index = table->hash_func(data) % table->capacity;
    bucket = table->buckets[index];

    assert(bucket);

    iter = SLLFind(SLLBegin(bucket), SLLEnd(bucket), table->is_match, (void*)data);

    if (SLLIsEqual(iter, SLLEnd(bucket)))
    {
        return NULL; 
    }

    found_data = SLLGetData(iter);

    SLLRemove(iter);
    SLLInsert(SLLBegin(bucket), found_data);

    return found_data;
}

double HashTableLoadFactor(const hash_table_t* table)
{
    size_t num_elements;

    assert(table);

    num_elements = HashTableSize(table);
    return ((double)num_elements) / table->capacity;
}

#ifndef ILRD_HASH_TABLE_H
#define ILRD_HASH_TABLE_H

#include <stddef.h> /* size_t */

#include "sll.h" /* SLLIsEmpty */

typedef size_t (*hash_func_t)(const void* data);


typedef struct hash_table hash_table_t;

 struct hash_table
{
    size_t capacity;
    hash_func_t hash_func;
    is_match_func_t is_match;
    sll_t** buckets;
};

/* O(n) where n is number of buckets */
hash_table_t* HashTableCreate(hash_func_t hash_func, size_t capacity ,is_match_func_t is_match); 

/* O(n+m) where n is size m is cap*/
void HashTableDestroy(hash_table_t* table);

/* O(1)  If a duplicate key is provided, undefined behavior  */
int HashTableInsert(hash_table_t* table, void* data);

/* O(1) WC-O(n)*/
void HashTableRemove(hash_table_t* table, const void* data);

/* O(1) WC-O(n) */
void* HashTableFind(const hash_table_t* table, const void* data); 

/* O(n+m) */
size_t HashTableSize(const hash_table_t* table);

/* O(n) */
int HashTableIsEmpty(const hash_table_t* table);

/* O(n+m) */
int HashTableForEach(hash_table_t* table, action_func_t action_func, void* param);

void* HashTableFindMoveFront(hash_table_t* table, const void* data);

double HashTableLoadFactor(const hash_table_t* table);

#endif /* ILRD_HASH_TABLE_H */









    
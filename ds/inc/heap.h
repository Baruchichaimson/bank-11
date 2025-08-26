#ifndef __ILRD_HEAP__   
#define __ILRD_HEAP__

typedef int (*is_match_func)(const void* data1, const void* data2);
typedef int (*compare_func)(const void* data1, const void* data2);
typedef struct heap heap_t;

heap_t* HeapCreate(compare_func func);

void HeapDestroy(heap_t* heap);

void* HeapPeek(const heap_t* heap);

int HeapPush(heap_t* heap, void* data);

void HeapPop(heap_t* heap);

void HeapRemove(heap_t* heap, const void* data, is_match_func func);

int HeapIsEmpty(const heap_t* heap);

size_t HeapSize(const heap_t* heap);

#endif /* __ILRD_HEAP__ */
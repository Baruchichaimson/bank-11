#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */ 

#include "bst.h" /* API */

static void swap(void* data1, void* data2);
static bst_iter_t NodeToIter(bst_node_t* node);

typedef enum child_node_pos
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} child_node_pos_t;

struct bst_node
{
    bst_node_t* parent;
    bst_node_t* children[NUM_OF_CHILDREN];
    void* data;
};

struct bst
{
    bst_node_t root_stub;    
    cmp_func_t cmp;
};

bst_t* BSTCreate(cmp_func_t cmp)
{
    bst_t* tree = NULL;

    assert(cmp);

    tree = (bst_t*)malloc(sizeof(bst_t));
    if (!tree)
    {
        return NULL;
    }

    tree->root_stub.parent = NULL;
    tree->root_stub.children[LEFT] = NULL;
    tree->root_stub.children[RIGHT] = NULL;
    tree->root_stub.data = NULL;
    tree->cmp = cmp;

    return tree;
}

void BSTDestroy(bst_t* tree)
{
    bst_node_t* current = tree->root_stub.children[LEFT];
    bst_node_t* prev = &tree->root_stub;  
    bst_node_t* next = NULL;

    assert(tree);

    if (BSTIsEmpty(tree))
    {
        free(tree);
        return;
    }

    while (current->parent != NULL)
    {
        if (prev == current->parent)
        {
            if (current->children[LEFT] != NULL)
            {
                next = current->children[LEFT];
            }
            else if (current->children[RIGHT] != NULL)
            {
                next = current->children[RIGHT];
            }
            else
            {
                next = current->parent;
                free(current);
                current = NULL;
            }
        }
        else if (prev == current->children[LEFT])
        {
            if (current->children[RIGHT] != NULL)
            {
                next = current->children[RIGHT];
            }
            else
            {
                next = current->parent;
                free(current);
                current = NULL; 
            }
        }
        else if (prev == current->children[RIGHT])
        {
            next = current->parent;
            free(current);
            current = NULL;
        }

        if(!current)
        {
            current = next;
            prev = next->children[RIGHT];
            continue;
        }
        prev = current;
        current = next;
    }

    free(tree);
}

bst_iter_t BSTInsert(bst_t* tree, void* data)
{
    bst_node_t* new_node = NULL;
    bst_node_t* parent = &tree->root_stub;
    bst_node_t* current = tree->root_stub.children[LEFT];

    assert(tree);
    assert(data);

    new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
    if (!new_node)
    {
        return (bst_iter_t){NULL};
    }

    new_node->data = data;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    if (current == NULL)
    {
        tree->root_stub.children[LEFT] = new_node;
        new_node->parent = &tree->root_stub;
        return (bst_iter_t){new_node};
    }

    while (current != NULL)
    {
        parent = current;

        if (tree->cmp(data, current->data) < 0)
        {
            current = current->children[LEFT];
        }
        else
        {
            current = current->children[RIGHT];
        }
    }

    new_node->parent = parent;

    if (tree->cmp(data, parent->data) < 0)
    {
        parent->children[LEFT] = new_node;
    }
    else
    {
        parent->children[RIGHT] = new_node;
    }

    return (bst_iter_t){new_node};
}

void BSTRemove(bst_iter_t to_remove)
{
    bst_iter_t successor = {NULL};

    assert(to_remove.node);

    if (to_remove.node->children[LEFT] && to_remove.node->children[RIGHT])
    {
        successor = BSTNext(to_remove);
        swap(to_remove.node->data, successor.node->data);
    }
    else if (to_remove.node->children[RIGHT])
    {
        successor = BSTNext(to_remove);
        swap(to_remove.node->data, successor.node->data);
    }
    else if (to_remove.node->children[LEFT])
    {
        successor = BSTNext(to_remove);
        swap(to_remove.node->data, successor.node->data);
    }
    else
    {
        successor = to_remove; 
    }

    if (successor.node->parent->children[LEFT] == successor.node)
    {
        successor.node->parent->children[LEFT] = NULL;
    }
    else if (successor.node->parent->children[RIGHT] == successor.node)
    {
        successor.node->parent->children[RIGHT] = NULL;
    }

    free(successor.node);   
}

size_t BSTSize(const bst_t* tree)
{   
    size_t size = 0;
    bst_iter_t iter = BSTBegin(tree);
    
    assert(tree);

    while (!BSTIterIsSame(iter, BSTEnd(tree)))
    {
        size++;
        iter = BSTNext(iter);
    }

    return size;
}

int BSTIsEmpty(const bst_t* tree)
{
    assert(tree);

    return (tree->root_stub.children[LEFT] == NULL);
}

bst_iter_t BSTBegin(const bst_t* tree)
{
    bst_node_t* current = tree->root_stub.children[LEFT];

    assert(tree);

    while (current && current->children[LEFT])
    {
        current = current->children[LEFT];
    }

    return NodeToIter(current);
}

bst_iter_t BSTEnd(const bst_t* tree)
{
    return NodeToIter(tree->root_stub.parent);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    bst_iter_t prev = iter;
    
    if(iter.node->children[LEFT])
    {
        prev.node = iter.node->children[LEFT];
        while (prev.node->children[RIGHT])
        {
            prev.node = prev.node->children[RIGHT];
        }
    }
    else 
    {
        while (iter.node->parent && iter.node->parent->children[RIGHT] != iter.node)
        {
            iter = (bst_iter_t){iter.node->parent};
        }
        prev.node = iter.node->parent;
    }
    return prev;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    bst_node_t* next = iter.node;

    if(iter.node->children[RIGHT])
    {
        next = iter.node->children[RIGHT];
        while (next->children[LEFT])
        {
            next = next->children[LEFT];
        }
    }
    else 
    {
        while (iter.node->parent && iter.node->parent->children[LEFT] != iter.node)
        {
            iter = (bst_iter_t){iter.node->parent};
        }
        next = iter.node->parent;
    }
    return NodeToIter(next);
}

int BSTIterIsSame(bst_iter_t iter1, bst_iter_t iter2)
{
    return iter1.node == iter2.node;
}

void* BSTGetData(bst_iter_t iter)
{
    return iter.node->data;
}

bst_iter_t BSTFind(const bst_t* bst, void* to_find)
{
    bst_node_t* current = bst->root_stub.children[LEFT];

    assert(bst);
    assert(to_find);

    while (current)
    {
        int cmp_result = bst->cmp(to_find, current->data);
        if (cmp_result == 0)
        {
            return NodeToIter(current);
        }
        else if (cmp_result < 0)
        {
            current = current->children[LEFT];
        }
        else
        {
            current = current->children[RIGHT];
        }
    }

    return (bst_iter_t){NULL}; 
}

int BSTForEach(bst_iter_t from, bst_iter_t to, int(*action_func)(void* data, void* param), void* param)
{
    bst_node_t* current = from.node;

    assert(action_func);

    while (current && !BSTIterIsSame((bst_iter_t){current}, to))
    {
        if (action_func(current->data, param) != 0)
        {
            return -1;
        }
        current = BSTNext((bst_iter_t){current}).node;
    }

    return 0;
}

static bst_iter_t NodeToIter(bst_node_t* node)
{
    return (bst_iter_t){node};
}

static void swap(void* data1, void* data2)
{
    int temp = *(int*)data1;
    *(int*)data1 = *(int*)data2;
    *(int*)data2 = temp;
}
   
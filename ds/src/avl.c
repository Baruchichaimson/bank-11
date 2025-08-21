/************************************
 * Exercise: AVL 
 * Date: 18/08/25
 * Developer: Baruch Haimson
 * Reviewer: Menny
 * Status: Aprroved
 ************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "avl.h" /* API */

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct avl_node avl_node_t;

typedef enum children
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} children_t;

struct avl
{
   avl_node_t* root;
   cmp_func_t cmp;
};

struct avl_node
{
    size_t height;
    void* data;
    struct avl_node* children[NUM_OF_CHILDREN];
};

/*==================== Helper Functions ============================================*/

static void DestroyRec(avl_node_t* node);
static size_t NodeHeight(avl_node_t* node);
static int Balance(avl_node_t* node);
static avl_node_t* RotateRight(avl_node_t* y);
static avl_node_t* RotateLeft(avl_node_t* node);
static void UpdateHeight(avl_node_t* node);
static avl_node_t* RemoveRec(avl_node_t* node, void* data, cmp_func_t cmp);
static avl_node_t* InsertRec(avl_node_t* node, void* data, cmp_func_t cmp, int* status);
static size_t CountRec(avl_node_t* node);
static void* AVLFindRec(avl_node_t* node, void* data, cmp_func_t cmp);
static int ForEachRec(avl_node_t* node, int traversal_type, action_func_t action_func, void* param);

/*==================================================================================*/

avl_t* AVLCreate(cmp_func_t cmp_func)
{
    avl_t* avl = NULL;

    avl = (avl_t*)malloc(sizeof(avl_t));
    if(!avl)
    {
        return NULL;
    }

    avl->root = NULL;
    avl->cmp = cmp_func;

    return avl;
}

void AVLDestroy(avl_t* tree)
{
    assert(tree);

    DestroyRec(tree->root);

    free(tree);
}

void AVLRemove(avl_t* tree, const void* data)
{
    assert(tree);
    
    tree->root = RemoveRec(tree->root, (void*)data, tree->cmp);
}

int AVLInsert(avl_t* tree, void* data)
{
    int status = 0;

    assert(tree);

    tree->root = InsertRec(tree->root, data, tree->cmp, &status);

    return status;
}

size_t AVLHeight(const avl_t* tree)
{
    return NodeHeight(tree->root);
}

size_t AVLCount(const avl_t* tree)
{
    assert(tree);

    return CountRec(tree->root);
}

int AVLIsEmpty(const avl_t* tree)
{
    return (!tree->root);
}

void* AVLFind(const avl_t* tree, const void* data)
{
    assert(tree);

    return AVLFindRec(tree->root, (void*)data, tree->cmp);
}

int AVLForEach(avl_t* tree, int traversal_type, action_func_t action_func, void* param)
{
    assert(tree);
    assert(action_func);

    return ForEachRec(tree->root, traversal_type, action_func, param);
} 

/*==================== Helper Functions ============================================*/

static void DestroyRec(avl_node_t* node)
{
    if (!node)
    {
        return;
    }

    DestroyRec(node->children[LEFT]);
    DestroyRec(node->children[RIGHT]);

    free(node);
}


static size_t NodeHeight(avl_node_t* node)
{
    return (!node) ? 0 : node->height;
}

static avl_node_t* InsertRec(avl_node_t* node, void* data, cmp_func_t cmp, int* status)
{
    avl_node_t* new_node = NULL;
    int cmp_res = 0;
    int balance = 0;
    *status = 0;

    if (!node)
    {
        new_node = (avl_node_t*)malloc(sizeof(avl_node_t));
        if (!new_node)
        {
            return NULL;
        }

        new_node->data = data;
        new_node->children[LEFT] = NULL;
        new_node->children[RIGHT] = NULL;
        new_node->height = 1;

        return new_node;
    }

    cmp_res = cmp(data, node->data);

    if (cmp_res < 0)
    {
        node->children[LEFT] = InsertRec(node->children[LEFT], data, cmp, status);
    }
    else if (cmp_res > 0)
    {
        node->children[RIGHT] = InsertRec(node->children[RIGHT], data, cmp, status);
    }
    else
    {
        *status = 1; 
        return node;
    }

    UpdateHeight(node);

    balance = Balance(node);

    if (balance > 1 && cmp(data, node->children[LEFT]->data) < 0)
    {
        return RotateRight(node);
    }

    if (balance < -1 && cmp(data, node->children[RIGHT]->data) > 0)
    {
        return RotateLeft(node);
    }

    if (balance > 1 && cmp(data, node->children[LEFT]->data) > 0)
    {
        node->children[LEFT] = RotateLeft(node->children[LEFT]);
        return RotateRight(node);
    }

    if (balance < -1 && cmp(data, node->children[RIGHT]->data) < 0)
    {
        node->children[RIGHT] = RotateRight(node->children[RIGHT]);
        return RotateLeft(node);
    }

    return node; 
}

static avl_node_t* RemoveRec(avl_node_t* node, void* data, cmp_func_t cmp)
{
    avl_node_t* successor = NULL;
    avl_node_t* child = NULL;
    int cmp_res = 0;
    int balance = 0;

    if (node == NULL)
    {
        return NULL;
    }

    cmp_res = cmp(data, node->data);

    if (cmp_res < 0)
    {
        node->children[LEFT] = RemoveRec(node->children[LEFT], data, cmp);
    }
    else if (cmp_res > 0)
    {
        node->children[RIGHT] = RemoveRec(node->children[RIGHT], data, cmp);
    }
    else 
    {
        if (node->children[LEFT] == NULL || node->children[RIGHT] == NULL)
        {
            child = node->children[LEFT] ? node->children[LEFT] : node->children[RIGHT];

            free(node);
            return child; 
        }
        else
        {
            successor = node->children[RIGHT];
            while (successor->children[LEFT])
            {
                successor = successor->children[LEFT];
            }

            node->data = successor->data;

            node->children[RIGHT] = RemoveRec(node->children[RIGHT], successor->data, cmp);
        }
    }

    UpdateHeight(node);

    balance = Balance(node);

    if (balance > 1 && Balance(node->children[LEFT]) >= 0)
    {
        return RotateRight(node);
    }
    if (balance > 1 && Balance(node->children[LEFT]) < 0)
    {
        node->children[LEFT] = RotateLeft(node->children[LEFT]);
        return RotateRight(node);
    }

    if (balance < -1 && Balance(node->children[RIGHT]) <= 0)
    {
        return RotateLeft(node);
    }
    if (balance < -1 && Balance(node->children[RIGHT]) > 0)
    {
        node->children[RIGHT] = RotateRight(node->children[RIGHT]);
        return RotateLeft(node);
    }


    return node;
}


static int Balance(avl_node_t* node)
{
    if (!node) 
    { 
        return 0; 
    }

    return (NodeHeight(node->children[LEFT]) - NodeHeight(node->children[RIGHT]));
}

static avl_node_t* RotateRight(avl_node_t* node)
{
    avl_node_t* lnode = node->children[LEFT];
    avl_node_t* lrnode = lnode->children[RIGHT];

    lnode->children[RIGHT] = node;
    node->children[LEFT] = lrnode;

    UpdateHeight(node);
    UpdateHeight(lnode);

    return lnode; 
}

static avl_node_t* RotateLeft(avl_node_t* node)
{
    avl_node_t* rnode = node->children[RIGHT];
    avl_node_t* rlnode = rnode->children[LEFT];

    rnode->children[LEFT] = node;
    node->children[RIGHT] = rlnode;

    UpdateHeight(node);
    UpdateHeight(rnode);

    return rnode; 
}

static void UpdateHeight(avl_node_t* node)
{
    if (!node) 
    {
        return;
    }

    node->height = 1 + MAX(NodeHeight(node->children[LEFT]), NodeHeight(node->children[RIGHT]));
}

static size_t CountRec(avl_node_t* node)
{
    if (!node)
    {
        return 0;
    }

    return 1 + CountRec(node->children[LEFT]) + CountRec(node->children[RIGHT]);
}

static void* AVLFindRec(avl_node_t* node, void* data, cmp_func_t cmp)
{
    int cmp_res = 0;

    if (!node) 
    {
        return NULL;
    }

    cmp_res  = cmp(data, node->data);

    if (cmp_res == 0) 
    {
        return node->data;;
    }
    else if (cmp_res < 0)
    {
        return AVLFindRec(node->children[LEFT], data, cmp);
    }
    else
    {
        return AVLFindRec(node->children[RIGHT], data, cmp);
    }
}

static int ForEachRec(avl_node_t* node, int travers, action_func_t action_func, void* param)
{
    int res = 0;

    if (!node) 
    {
        return 0;
    }

    if (travers == AVL_PRE_ORDER)
    {
        res = action_func(node->data, param);

        if (res) return res;
    }

    res = ForEachRec(node->children[LEFT], travers, action_func, param);

    if (res) 
    {
        return res;
    }

    if (travers == AVL_IN_ORDER)
    {
        res = action_func(node->data, param);
        if (res) 
        {
            return res;
        }
    }

    res = ForEachRec(node->children[RIGHT], travers, action_func, param);

    if (res) 
    {
        return res;
    }

    if (travers == AVL_POST_ORDER)
    {
        res = action_func(node->data, param);
    }

    return res;
}

avl_node_t* AVLNext(avl_node_t* node)
{
    avl_node_t* next = NULL;

    assert(node);

    next = node;
    
    if(node->children[RIGHT])
    {
        next = node->children[RIGHT];
        while (next->children[LEFT])
        {
            next = next->children[LEFT];
        }
    }

    return next;
}
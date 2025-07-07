#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct node
{
	void* data;
	struct node *next;
}node_t;


node_t *Flip(node_t *head)
{
    node_t *tmp = NULL;
    node_t *curr = head;
    node_t *next = NULL;

    while (curr != NULL)
    {
        next = curr->next;    
        curr->next = tmp;     
        tmp = curr;           
        curr = next;           
    }

    return tmp;  
}

int Hashloop(const node_t *head)
{
    const node_t *slow = head;
    const node_t *fast = head;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return 1; 
        }
    }

    return 0; 
}

size_t Length(const node_t *head)
{
    size_t count = 0;
    while (head)
    {
        ++count;
        head = head->next;
    }
    return count;
}

node_t *Findinstruction(node_t *head1, node_t *head2)
{
    size_t len1 = Length(head1);
    size_t len2 = Length(head2);

    node_t *longer = (len1 > len2) ? head1 : head2;
    node_t *shorter = (len1 > len2) ? head2 : head1;

    size_t diff = len1 > len2 ? len1 - len2 : len2 - len1;

    while (diff--)
    {
        longer = longer->next;
    }

    while (longer && shorter)
    {
        if (longer == shorter)
        {
            return longer;
        }

        longer = longer->next;
        shorter = shorter->next;
    }

    return NULL;
}


/* ----------------- help ----------------- */

node_t *CreateNode(int value)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    int *data = (int *)malloc(sizeof(int));
    *data = value;
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void FreeList(node_t *head)
{
    node_t *temp = NULL;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

void PrintList(node_t *head)
{
    while (head)
    {
        printf("%d -> ", *(int*)(head->data));
        head = head->next;
    }
    printf("NULL\n");
}


int main()
{
    /* ===== Flip ===== */
    node_t *head = CreateNode(1);
    head->next = CreateNode(2);
    head->next->next = CreateNode(3);
    head->next->next->next = CreateNode(4);

    printf("Original list: ");
    PrintList(head);

    head = Flip(head);
    printf("Flipped list:  ");
    PrintList(head);

    FreeList(head);

    /* =====  Hasloop ===== */
    node_t *loop_list = CreateNode(1);
    loop_list->next = CreateNode(2);
    loop_list->next->next = CreateNode(3);
    loop_list->next->next->next = CreateNode(4);
    loop_list->next->next->next->next = loop_list->next;

    if (Hashloop(loop_list))
    {
        printf("\nHashloop: Loop detected \n\n");
    }
    else
    {
        printf("\nHashloop: No loop \n\n");
    }


    /* =====Findinstruction ===== */
    node_t *common = CreateNode(99);
    common->next = CreateNode(100);

    node_t *list1 = CreateNode(1);
    list1->next = CreateNode(2);
    list1->next->next = common;

    node_t *list2 = CreateNode(3);
    list2->next = common;

    printf("List1: ");
    PrintList(list1);
    printf("List2: ");
    PrintList(list2);

    node_t *intersection = Findinstruction(list1, list2);
    if (intersection)
    {
        printf("Intersection found at node with data: %d \n", *(int *)intersection->data);
    }
    else
    {
        printf("No intersection \n");
    }

    FreeList(list1); 
  
    free(list2->data);
    free(list2);


    return 0;
}

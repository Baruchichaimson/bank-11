#include <iostream>
#include <vector>
using namespace std;

/* ---------------- ListNode definition ---------------- */
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

/* ---------------- Solution (paste your solution here) ---------------- */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) 
    {
        int sum;
        int carry = 0;

        ListNode* hl1 = flip(l1);
        ListNode* hl2 = flip(l2);

        ListNode* result = new ListNode(0); /* dummy head */
        ListNode* hresult = result;

        while (hl1 != nullptr || hl2 != nullptr || carry != 0)
        {
            int v1 = hl1 ? hl1->val : 0;
            int v2 = hl2 ? hl2->val : 0;

            sum = v1 + v2 + carry;
            carry = sum / 10;

            result->next = new ListNode(sum % 10);
            result = result->next;

            if (hl1) hl1 = hl1->next;
            if (hl2) hl2 = hl2->next;
        }

        return flip(hresult->next);
    }

    ListNode* addTwoNumbersNoFlip(ListNode* l1, ListNode* l2) 
    {
        int sum;
        int carry = 0;

        ListNode* result = new ListNode(0); /* dummy head */
        ListNode* hresult = result;

        addTwoNumbersNoFlip(l1->next, l2->next);

        int v1 = hl1 ? hl1->val : 0;
        int v2 = hl2 ? hl2->val : 0;

        sum = v1 + v2 + carry;
        carry = sum / 10;
        
        result->next = new ListNode(sum % 10);
        result = result->next;
    }

private:
    ListNode* flip(ListNode* head)
    {
        ListNode* curr = head;
        ListNode* prev = nullptr;
        ListNode* next = nullptr;

        while (curr != nullptr)
        {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};

/* ---------------- Helper Functions ---------------- */

/* ---- Create linked list from vector ---- */
ListNode* CreateList(const vector<int>& values)
{
    ListNode* head = nullptr;
    ListNode* tail = nullptr;

    for (int v : values)
    {
        ListNode* node = new ListNode(v);
        if (!head)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

/* ---- Print linked list ---- */
void PrintList(ListNode* head)
{
    while (head)
    {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

/* ---- Free linked list ---- */
void FreeList(ListNode* head)
{
    while (head)
    {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

/* ---------------- Tests ---------------- */
int main()
{
    Solution sol;

    /* Test 1: 342 + 465 = 807 */
    ListNode* l1 = CreateList({2,4,3});
    ListNode* l2 = CreateList({5,6,4});

    cout << "Test 1:" << endl;
    PrintList(sol.addTwoNumbers(l1, l2)); /* expected: 7 -> 0 -> 8 */

    FreeList(l1);
    FreeList(l2);

    /* Test 2: 0 + 0 = 0 */
    l1 = CreateList({0});
    l2 = CreateList({0});

    cout << "Test 2:" << endl;
    PrintList(sol.addTwoNumbers(l1, l2)); /* expected: 0 */

    FreeList(l1);
    FreeList(l2);

    /* Test 3: 999 + 1 = 1000 */
    l1 = CreateList({9,9,9});
    l2 = CreateList({1});

    cout << "Test 3:" << endl;
    PrintList(sol.addTwoNumbers(l1, l2)); /* expected: 0 -> 0 -> 0 -> 1 */

    FreeList(l1);
    FreeList(l2);

    /* Test 4: different lengths */
    l1 = CreateList({1,8});   /* 81 */
    l2 = CreateList({0});     /* 0 */

    cout << "Test 4:" << endl;
    PrintList(sol.addTwoNumbers(l1, l2)); /* expected: 1 -> 8 */

    FreeList(l1);
    FreeList(l2);

    return 0;
}

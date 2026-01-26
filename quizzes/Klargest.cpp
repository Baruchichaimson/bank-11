#include <queue>
#include <vector>
#include <iostream>
#include <functional>

int findKthLargest(int arr[], int size, int k)
{
    std::priority_queue<
        int,
        std::vector<int>,
        std::greater<int>
    > minHeap;

    for (int i = 0; i < size; i++)
    {
        minHeap.push(arr[i]);

        if (minHeap.size() > k)
        {
            minHeap.pop();
        }
    }

    return minHeap.top();
}

int main()
{
    int arr[] = {3,2,3,1,2,4,5,5,6};
    int size = sizeof(arr) / sizeof(arr[0]);

    int result = findKthLargest(arr, size, 4);
    std::cout << result << std::endl;

    return 0;
}

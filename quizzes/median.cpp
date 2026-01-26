#include <iostream>
#include <vector>
#include <queue>

class MedianFinder 
{
    public:

        void addNumber(int x);
        double getMedian();

    private:

        void fixBalance();
        std::priority_queue< int > MaxHeap;
        std::priority_queue< int, std::vector<int>, std::greater<int> > minHeap;         
};

void MedianFinder::addNumber(int x)
{
    if(MaxHeap.size() == 0 && minHeap.size() == 0)
    {
        minHeap.push(x);
    }
    else
    {
        if(x < minHeap.top())
        {
            MaxHeap.push(x);
        }
        else
        {
            minHeap.push(x);
        }
    }
    
    fixBalance();
}
double MedianFinder::getMedian()
{
    if(MaxHeap.size() > minHeap.size())
    {
        return MaxHeap.top();
    }
    else if(MaxHeap.size() < minHeap.size())
    {
        return minHeap.top();
    }

    return (minHeap.top() + MaxHeap.top()) / 2.0;
}

void MedianFinder::fixBalance()
{
    if(MaxHeap.size() > minHeap.size() + 1)
    {
        minHeap.push(MaxHeap.top());
        MaxHeap.pop();
    }
    else if(minHeap.size() > MaxHeap.size() + 1)
    {
        MaxHeap.push(minHeap.top());
        minHeap.pop();
    }
}

int main()
{
    MedianFinder mf;

    // int input = 0; 
    // while (true)
    // {
    //     std::cin >> input;

    //     if (input == -1)
    //     {
    //         break;
    //     }

    //     try
    //     {
    //         mf.addNumber(input);
    //     }
    //     catch (...)
    //     {
    //         std::cout << "Invalid input\n";
    //     }
    // }

    mf.addNumber(2);
    mf.addNumber(3);
    mf.addNumber(6);
    mf.addNumber(1);

    double result = mf.getMedian();
    std::cout << result << std::endl;
    return 0;
}
#include <iostream>
#include <vector>


int fast_solution(std::vector<int>& client)
{
    int largest = 0;
    int count = 0;
    int maxcount = 0;

    for(int i = 0; i < client.size(); i++)
    {
        int diff = client[i] - largest - 1;
        if(client[i] > largest)
        {
            count += diff;
            if(maxcount < count)
            {
                maxcount = count;
            }
            largest = client[i];
        }
        else
        {
            count--;
        }
        
    }
    return maxcount;
}
int solution(std::vector<int>& client)
{
    int count = 0;
    int shelf[client.size()] = {0};
    int num = client.front();

    for(int i = 1; i < num; i++)
    {
        shelf[i - 1] = 1;  
        count++;
    }    
    shelf[num - 1] = -1;

    int maxcount = count;

    for(int i = 1; i < client.size(); i++)
    {
        if (client[i] > client[i - 1])
        {
            for(int j = client[i - 1]; j < client[i] - 1; j++)
            {
                if(shelf[j] == 0)
                {
                    shelf[j] = 1;
                    count++;
                } 
            }
            shelf[client[i]] = -1;
            if(maxcount < count)
            {
                maxcount = count;
            }
        }
        else 
        {
            shelf[client[i] - 1] = -1;
            count--;
        }
    }

    return maxcount;
}

int main()
{
    int result1 = 0;
    int result2 = 0;
    std::vector<int> client1 = {1,2,3}; 
    std::vector<int> client2 = {3,2,5,1,4};
    std::vector<int> client3 = {3,2,7,5,4,1,6};
    result1 = solution(client1);
    std::cout << result1 << std::endl;
    result2 = fast_solution(client1);
    std::cout << result2 << std::endl;
    result1 = solution(client2);
    std::cout << result1 << std::endl;
    result2 = fast_solution(client2);
    std::cout << result2 << std::endl;
    result1 = solution(client3);
    std::cout << result1 << std::endl;
    result2 = fast_solution(client3);
    std::cout << result2 << std::endl;

    return 0;
}
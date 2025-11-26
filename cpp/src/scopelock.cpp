/**************************************
Exercise: 	cpp_intro - rc_string
Date:		24/11/2025
Developer:	Baruch Haimson
Reviewer: 	shiran 
Status:		Approved
**************************************/

#include <iostream> /* cout */
#include <mutex> /* mutex */
#include <fstream> /* template */

namespace ilrd
{
    template<typename Resource>
    class RAII 
    {
    private:
        Resource& res;
    public:
        RAII(Resource& r) : res(r) 
        {
            res.lock();; 
            std::cout << "Resource acquired\n";
        }

        ~RAII() 
        {
            res.unlock(); 
            std::cout << "Resource released\n";
        }

        RAII(const RAII&) = delete;           // for prevent double unlock -> UB for example
        RAII& operator=(const RAII&) = delete;
    };

}
int main() 
{
    std::mutex myMutex;

    ilrd::RAII<std::mutex> lock(myMutex);
    std::cout << "Inside critical section\n";
    
    return 0;
}

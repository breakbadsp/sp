#include "CMN.hpp"
#include "cmn.hpp"

int CalculateSum(int a, int b, int c)
{
    return a + b + c;
}

void TestCmn()
{

    auto* new_thread = sp::CreateAndRunThread(
            0, 
            "TestThread",
            CalculateSum,
            10, 20, 30
            );

    if(!new_thread)
        return;

    new_thread->join();
    std::cout << "Exiting main\n";
    delete new_thread;   
}
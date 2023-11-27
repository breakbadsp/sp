#include <cassert>
 
#include "CMN.hpp"
#include "cmn.hpp"
#include "Types.hpp"

#define ASSERT(exp) exp ? std::cout << "Test case " << #exp << " passed\n" : \
                          std::cout << "Test case " << #exp << " failed!\n";

int CalculateSum(int a, int b, int c)
{
    return a + b + c;
}

void TestCmn()
{
  TestFloatingPointMath();
  TestThreadCreation();
  TestMoveAndForward();
  TestTypes();
}

void TestTypes()
{
  [[maybe_unused]]sp::size_t test = 1000;
}

void TestMoveAndForward()
{
  std::cout << "\n===========TestMoveAndForward==============\n";
  int a = 10;
  int b = sp::move(a);

  int c = sp::move(b);
  [[maybe_unused]] auto d = sp::forward(c);

  std::cout << d << '\n';
  (void)d;
  std::cout << "\n===========TestMoveAndForward==============\n";
}

void TestThreadCreation()
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

void TestFloatingPointMath()
{
  //Greater
  ASSERT(!sp::almost_equal(100.0009, 100.0008, 5));
  ASSERT(!sp::less_than(100.0009, 100.0008, 5));
  ASSERT(sp::greater_than(100.0009, 100.0008, 5));

  //less
  ASSERT(!sp::almost_equal(100.0007, 100.0008, 5));
  ASSERT(sp::less_than(100.0007, 100.0008, 5));
  ASSERT(!sp::greater_than(100.0007, 100.0008, 5));

  //Equal
  ASSERT(sp::almost_equal(100.0008, 100.0008, 5));
  ASSERT(!sp::less_than(100.0008, 0.0008, 5));
  ASSERT(!sp::greater_than(100.0008, 100.0008, 5));
}
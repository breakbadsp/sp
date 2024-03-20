#include <cassert>
#include <vector>
#include <thread>
#include <algorithm>
 
#include "CMN.hpp"
#include "cmn.hpp"
#include "Types.hpp"
#include "Hashing.hpp"
#include "spinlock.hpp"

#define ASSERT(exp) exp ? std::cout << "Test case " << #exp << " passed\n" : \
                          std::cout << "Test case " << #exp << " failed!\n";

int CalculateSum(int a, int b, int c)
{
    return a + b + c;
}

void TestCmn()
{
  /*TestFloatingPointMath();
  TestThreadCreation();
  TestMoveAndForward();
  TestTypes();
  TestHashingFunctions();*/
  TestLocks();
}

void TestTypes()
{
  [[maybe_unused]]sp::size_t test = 1000;
}

void TestHashingFunctions()
{
  ASSERT( (sp::Hash(105, 10) == 5) );
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


void Sum(size_t& p_data, size_t  p_begin, size_t p_end, sp::spinlock_v1& p_lock)
{
  for(size_t i = p_begin; i <  p_end; ++i)
  {
    p_lock.lock();
    p_data += i;
    p_lock.unlock();
  }
}

void TestLocks()
{
  size_t sum_by_t1 = 0;
  size_t sum_by_t2 = 0;
  std::vector<std::thread*> workers;
  std::cout << "Test lock , initial count " << sum_by_t1 << '\n';
  std::cout << "Test lock , initial count " << sum_by_t2 << '\n';

  sp::spinlock_v1 lock;
  auto* new_thread1 = sp::CreateAndRunThread(
            1, 
            "TestThread",
            Sum,
            sum_by_t1, 1, 999999, lock
            );

  auto* new_thread2 = sp::CreateAndRunThread(
            1, 
            "TestThread",
             Sum,
            sum_by_t1, 1000000, 999999999999, lock
            );

    if(!new_thread1 || !new_thread2)
        return;

  size_t total = 0;
  for(size_t i = 1; i < 999999999999; ++i)
    total += i;

  std::cout << "Test lock , Final count " << sum_by_t1 
      << ", expected count " << total <<  '\n';

    new_thread1->join();
    new_thread2->join();
    delete new_thread1;
    delete new_thread2;
  std::cout << "Exiting main\n";
}
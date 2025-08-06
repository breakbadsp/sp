#include <cassert>
#include <vector>
#include <thread>
#include <algorithm>

#include "CMN.hpp"
#include "Hashing.hpp"
#include "Types.hpp"
#include "cmn.hpp"
#include "latch.hpp"
#include "owning_ptr.hpp"
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
  //TestThreadCreation();
  //TestLocks();
  TestLatch();
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

    auto new_thread = sp::CreateAndRunThread(
            0, 
            "TestThread",
            CalculateSum,
            10, 20, 30
            );

    if(!new_thread)
        return;

    new_thread->join();
    std::cout << "Exiting main\n";
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


void Sum(size_t* p_data, size_t  p_begin, size_t p_end, sp::spinlock_v1& p_lock)
{
  for(size_t i = p_begin; i <= p_end; ++i)
  {
    p_lock.lock(); // if we remove locking TSAN gives error
    *p_data += i;
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

  constexpr size_t TOTAL = 200;

  sp::spinlock_v1 lock;
  auto new_thread1 = sp::CreateAndRunThread(
            1, 
            "TestThread1",
            Sum,
            &sum_by_t1, 1, TOTAL/2, lock
            );

  auto new_thread2 = sp::CreateAndRunThread(
            2, 
            "TestThread2",
             Sum,
            &sum_by_t1, TOTAL/2 + 1, TOTAL, lock
            );
  
    std::cout << "Both workers started\n";

    if(!new_thread1 || !new_thread2)
        return;

  std::cout << "Calculating expected\n";
  size_t total = 0;
  for(size_t i = 1; i <= TOTAL; ++i)
    total += i;  

  if(new_thread1)
    new_thread1->join();
  if(new_thread2)
    new_thread2->join();

 std::cout << "Test lock , Final count " << sum_by_t1 
  << ", expected count " << total <<  '\n';
  std::cout << "Exiting main\n";
}

void TestLatch()
{
  std::cout << "Latch test started\n";
  constexpr int total_threads = 10;
  std::atomic_int num = 0;
  std::vector<sp::owning_ptr<std::thread>> threads;
  sp::latch latch(total_threads);
  sp::latch latch2(total_threads);

  for(int i =0; i < total_threads; ++i) {
    std::string thread_num = "thread";
    thread_num += std::to_string(i);
    auto thread = sp::CreateAndRunThread(0, thread_num, 
      [&latch, &latch2, &num, thread_name = thread_num] ()
      {
        int tnum = ++num;
        std::cout << "Started running " << thread_name << " at " << tnum << '\n';
        if(tnum != total_threads)
        {
          latch.decreament_count();
          latch.wait();
          std::cout << "released " << tnum << " thread\n";
          latch2.decreament_count();
        }
        else
        {
          std::cout <<  "Started running " << thread_name << " at final " << tnum << '\n';
          latch.decreament_count();
          std::cout << "Started releasing other threads\n";

          latch2.decreament_count();
          latch2.wait();
          std::cout << "Done releasing other threads\n";
        }
      }
    );
    if(thread)
      threads.push_back(thread);
  }

  std::cout << "Main thread Waiting for workers to finish!\n";
  for(auto* t: threads) {
    t->join();
  }
}
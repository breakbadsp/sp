
#include "VectorTests.hpp"
#include "StringTest.hpp"
#include "ListTest.hpp"
#include "QueueTest.hpp"
#include "TestSharedPtr.hpp"
#include "MapTest.hpp"
#include "CMN.hpp"
#include "cmn.hpp"
#include "OrderBookTests.hpp"
#include "HashSetTests.hpp"
#include "MemPoolTest.hpp"
#include "mpscllqueue.hpp"

int main()
{
  sp::mpscllqueue<int> llq;
  TestCmn();
  return 0;
  
  TestMemPool();
  TestOrderBook();
  TestHashSet();
  
  using namespace sp;
  std::vector<std::thread*> threads;
  threads.emplace_back(CreateAndRunThread(1, "TestVectorPushBack", TestVectorPushBack));
  threads.emplace_back(CreateAndRunThread(2, "TestListApis", TestListApis));
  threads.emplace_back(CreateAndRunThread(3, "TestString", TestString));
  threads.emplace_back(CreateAndRunThread(4, "TestSharedPtr", TestSharedPtr));
  threads.emplace_back(CreateAndRunThread(5, "TestMap", TestMap));

  for(auto* t: threads)
  {
    t->join();
    delete t;
  }
  
  std::cout << "test are done\n";
}

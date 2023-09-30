
#include "VectorTests.hpp"
#include "StringTest.hpp"
#include "ListTest.hpp"
#include "QueueTest.hpp"
#include "TestSharedPtr.hpp"
#include "MapTest.hpp"
#include "CMN.hpp"
#include "cmn.hpp"

int main()
{
  std::vector<std::thread*> threads;
  TestCmn();
  
  using namespace sp;
  //if(argc <= 1)
    //threads.emplace_back(CreateAndRunThread(-1, "TestQueue", TestQueue, 1));
  //else
    //threads.emplace_back(CreateAndRunThread(0, "TestQueue", TestQueue, atoi(argv[1])));
  /*
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
  */
  std::cout << "test are done\n";
}

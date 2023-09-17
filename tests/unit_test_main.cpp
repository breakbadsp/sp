
#include "VectorTests.hpp"
#include "StringTest.hpp"
#include "ListTest.hpp"
#include "QueueTest.hpp"
#include "TestSharedPtr.hpp"
#include "MapTest.hpp"

int main(int argc, char** argv)
{
  if(argc <= 1)
    TestQueue(1);
  else
    TestQueue(atoi(argv[1]));
  TestVectorContructor();
  TestVectorPushBack();
  TestListApis();
  TestString();
  TestSharedPtr();
  TestMap();

  std::cout << "test are done\n";
}

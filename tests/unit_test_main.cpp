
#include "VectorTests.hpp"
#include "ListTest.hpp"
#include "QueueTest.hpp"

int main(int argc, char** argv)
{
  if(argc <= 1)
    TestQueue(1);
  else
    TestQueue(atoi(argv[1]));
  TestVectorContructor();
  TestVectorPushBack();
  TestListApis();
  std::cout << "test are done\n";
}

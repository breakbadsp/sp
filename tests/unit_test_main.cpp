
#include "VectorTests.hpp"
#include "ListTest.hpp"
#include "QueueTest.hpp"
#include "shared_ptr.hpp"

struct Test {};
sp::shared_ptr<Test> global;

void TestSharedPtr()
{

  sp::shared_ptr sp_test(new Test());
  sp::shared_ptr sp2 = sp_test;
  sp::shared_ptr sp3 = sp_test;
  sp::shared_ptr sp4 = sp3;
  global = sp3;
}

int main(int argc, char** argv)
{
  if(argc <= 1)
    TestQueue(1);
  else
    TestQueue(atoi(argv[1]));
  TestVectorContructor();
  TestVectorPushBack();
  TestListApis();
  TestSharedPtr();
  std::cout << "test are done\n";
}

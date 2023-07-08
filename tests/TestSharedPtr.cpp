#include "shared_ptr.hpp"
#include "TestSharedPtr.hpp"

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

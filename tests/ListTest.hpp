#include "list.hpp"

void TestListApis()
{
  std::cout << "Link List test\n";
  sp::list obj;
  obj.push_front(10);
  std::cout << "push_front 10 \n";
  int param_1 = obj.get(1);
  std::cout << "At 1 = " << param_1 << std::endl;
  obj.Print();
  obj.push_back(20);
  std::cout << "push_back  20 \n";
  obj.Print();
  obj.insert_at(1,30);
  std::cout << "insert 30 at 1\n";
  obj.Print();
  obj.delete_at(1);
  std::cout << "delete at 1\n";
  obj.Print();
}
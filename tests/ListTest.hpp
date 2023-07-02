#pragma once
#include "list.hpp"
#include "iostream"

void TestListApis()
{
  std::cout << "Link List test\n";
  sp::list<int> obj;
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

  struct Test {
    int number;
  };

  Test t2;
  std::cout << "Link List test\n";
  sp::list<Test> list2;
  list2.push_front(Test());
  list2.get(1);
  list2.Print();
  list2.push_back(Test());
  list2.push_back(t2);
  list2.Print();
  list2.insert_at(1,Test());
  list2.Print();
  list2.delete_at(1);
  list2.Print();
}

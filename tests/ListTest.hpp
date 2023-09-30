#pragma once
#include "list.hpp"
#include "doubly_list.hpp"
#include "iostream"

void TestListApis()
{
  std::cout << "==========List test==========\n";
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
    int number {0};
  };

  Test t2;
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
/*
  std::cout << "============doubly_list test===============\n";
  sp::doubly_list<int> list3;
  list3.push_front(10);
  std::cout << "push_front 10 \n";
  int param_2 = list3.get(1);
  std::cout << "At 1 = " << param_2 << std::endl;
  list3.Print();
  list3.push_back(20);
  std::cout << "push_back  20 \n";
  list3.Print();
  list3.insert_at(1,30);
  std::cout << "insert 30 at 1\n";
  list3.Print();
  list3.delete_at(1);
  std::cout << "delete at 1\n";
  list3.Print();

  Test t3;
  sp::doubly_list<Test> list4;
  list4.push_front(Test());
  list4.get(1);
  list4.Print();
  list4.push_back(Test());
  list4.push_back(t3);
  list4.Print();
  list4.insert_at(1,Test());
  list4.Print();
  list4.delete_at(1);
  list4.Print();
  */
}

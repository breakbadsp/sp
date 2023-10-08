#pragma once

#include <iostream>
#include "hash_set.hpp"

void TestHashSet()
{
  sp::has_set<int> temp_set;
  temp_set.insert(1);
  temp_set.Print();
  temp_set.insert(2);
  temp_set.Print();
  temp_set.insert(3);
  temp_set.Print();
  temp_set.insert(4);
  temp_set.Print();
  temp_set.insert(5);
  temp_set.Print();
  temp_set.insert(6);
  temp_set.Print();

  if(!temp_set.find(3))
  {
    std::cout << "sp::hash_set::find test case failed\n";
  }
  temp_set.Print();

  if(temp_set.find(7))
  {
    std::cout << "sp::hash_set::find test case failed\n";
  }
  temp_set.Print();

  if(!temp_set.erase(1))
  {
    std::cout << "sp::hash_set::find test case failed\n";
  }
  temp_set.Print();

  if(!temp_set.erase(2))
  {
    std::cout << "sp::hash_set::find test case failed\n";
  }
  temp_set.Print();

  temp_set.insert(1);
  temp_set.Print();
  temp_set.insert(2);
  temp_set.Print();
  temp_set.insert(3);
  temp_set.Print();
  temp_set.insert(4);
  temp_set.Print();
  temp_set.insert(5);
  temp_set.Print();
  temp_set.insert(6);
  temp_set.Print();
}
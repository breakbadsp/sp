#pragma once
#include <iostream>
#include "string.hpp"

void TestString()
{
  sp::string test1  = "Test1 String sachin";
  sp::string test2("Test2 String sachin");
  std::cout << test1 << '\n';
  std::cout << test2 << '\n';
  std::cout << test1 + test2 << '\n';
  const auto new_str = test1 + test2;
  std::cout << new_str << '\n';

  const auto&& moved_string = std::move(new_str);
  std::cout << moved_string << " and moved from " << new_str << '\n';
}
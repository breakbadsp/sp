#pragma once
#include <iostream>
#include "string.hpp"



void func3()
{
        std::cout << "temp string \n";
        sp::string test("temp string ");
        sp::string test2(std::move(test));
        sp::string test3(test2);
        std::cout << "out: " << test3 << '\n';

        sp::string test4;
        sp::string test5;
        test4 = test3;
        test5 = std::move(test4);
        std::cout << test5 << '\n';
}

void func2()
    {
        std::cout << "Empty string2:" << '\n';
        sp::string test("");
        sp::string test2(std::move(test));
        sp::string test3(test2);
        sp::string test4;
        sp::string test5;
        test4 = test3;
        test5 = std::move(test4);
        std::cout << test5 << '\n';
    }

void func1()
    {
        std::cout << "Empty string1:" << '\n';
        sp::string test("");
        sp::string test2(std::move(test));
        
        sp::string test3(test2);
        sp::string test4;
        sp::string test5;
        test4 = test3;
        test5 = std::move(test4);
        std::cout << test << '\n';
        std::cout << "Empty string1 end." << '\n';
    }

void TestString()
{
  std::cout << "test starts\n";
  func1();
  func2();
  func3();
  
  sp::string test1  = "Test1 String sachin";
  sp::string test2("Test2 String sachin");
  std::cout << test1 << '\n';
  std::cout << test2 << '\n';
  
  std::cout << test1 + test2 << '\n';
  const auto new_str = test1 + test2;
  std::cout << "Concat:";
  std::cout << new_str << '\n';

  sp::string test3 = "Part";
  test3 += " first";
  test3 += " second";
  test3 += " third";
  std::cout << test3 << '\n';

  const auto&& moved_string = sp::move(new_str);
  std::cout << moved_string << '\n';
}
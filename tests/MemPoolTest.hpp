#pragma once

#include <iostream>
#include <cassert>
#include <type_traits>

#include "MemPool.hpp"

void TestMemPool()
{
  std::cout << "===============Testing MemPool===============\n";
  struct Test
  {
    int a;
  };

  sp::MemPool<int> int_mem_pool(10);
  sp::MemPool<Test> struct_mem_pool(10);
  auto* int_ptr = int_mem_pool.Allocate();
  //static_assert( std::is_same_v<decltype((*int_ptr)),int>);
  *int_ptr = 10;
  assert(*int_ptr == 10);
  std::vector<int*> numbers;
  for(size_t i =0; i<10; ++i)
  {
    auto* ptr = int_mem_pool.Allocate();
    assert(ptr != nullptr);
    numbers.push_back(ptr);
  }

  std::vector<Test*> structs;
  for(size_t i =0; i<10; ++i)
  {
    auto* ptr = struct_mem_pool.Allocate();
    assert(ptr != nullptr);
    structs.push_back(ptr);
  }

  for(auto* ptr: numbers)
  {
    int_mem_pool.Deallocate(ptr);
  }

  for(auto* ptr: structs)
  {
    struct_mem_pool.Deallocate(ptr);
  }
  numbers.clear();
  structs.clear();

  for(size_t i =0; i<10; ++i)
  {
    auto* ptr = int_mem_pool.Allocate();
    assert(ptr != nullptr);
    numbers.push_back(ptr);
  }
  for(size_t i =0; i<10; ++i)
  {
    auto* ptr = struct_mem_pool.Allocate();
    assert(ptr != nullptr);
    structs.push_back(ptr);
  }

  //TODO:: Add support to detect Mem Pool is full
  //auto* ptr2 = struct_mem_pool.Allocate();
  //assert(ptr2 == nullptr);
  //auto* ptr1 = int_mem_pool.Allocate();
  //assert(ptr1 == nullptr); 

  std::cout << "===============Testing MemPool===============\n";
}
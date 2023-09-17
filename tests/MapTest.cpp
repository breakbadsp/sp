#include "MapTest.hpp"
#include "map.hpp"

void TestMap() {
  std::cout << "=======================Testing sp::map starts==========================\n";
  sp::map<int, int> tmp1;
  tmp1.insert({5,1});
  tmp1.insert({2,1});
  tmp1.insert({3,1});
  tmp1.insert({4,1});
  tmp1.insert({1,1});
  tmp1.insert({6,1});
  tmp1.insert({7,1});
  tmp1.insert({8,1});
  tmp1.insert({9,1});
  tmp1.insert({10,1});
  tmp1.insert({11,1});
  tmp1.Print();
  std::cout << "=======================Testing sp::map Ends==========================\n";
}
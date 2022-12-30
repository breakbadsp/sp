#include "vector.hpp"

void TestVectorContructor()
{
  sp::vector<int> test_vec;
  test_vec.Print();
}

void TestVectorPushBack()
{
  std::cout << "Pushback test\n";
  sp::vector<int> test_vec(1);
  test_vec.Print();
  test_vec.push_back(2);
  test_vec.push_back(3);
  test_vec.push_back(4);
  test_vec.Print();
  std::cout << test_vec.back() << '\n';
  test_vec.pop_back();
  test_vec.Print();
}
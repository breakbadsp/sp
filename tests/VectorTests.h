#include "Vector.h"

void TestVectorContructor()
{
  sp::Vector<int> test_vec;
  test_vec.Print();
}

void TestVectorPushBack()
{
  std::cout << "Pushback test\n";
  sp::Vector<int> test_vec(1);
  test_vec.Print();
  test_vec.PushBack(2);
  test_vec.PushBack(3);
  test_vec.PushBack(4);
  test_vec.Print();
  std::cout << test_vec.Back() << '\n';
  test_vec.PopBack();
  test_vec.Print();
}
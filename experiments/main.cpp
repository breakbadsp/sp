#include "vector.hpp"

template<typename T>
void print(sp::vector<T>& p_vec)
{
  std::cout << "printing vec of size " << p_vec.size() << " and capacity " 
    << p_vec.capacity()  << std::endl;
  for(size_t i = 0; i < p_vec.size(); ++i)
  {
    std::cout << p_vec.at(i) << " ";
  }
  std::cout << ".\n";
}

int main()
{
  sp::vector<int> nums;
  sp::vector<int> nums10(10);

  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);

  print(nums);

  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  
  print(nums10);



  struct Test {
    Test() {
      std::cout << "Test\n";
    }
  };

  sp::vector<Test> tests(11);

}
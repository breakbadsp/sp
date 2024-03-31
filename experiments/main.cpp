#include "vector.hpp"



  struct Test {
    Test() {
      std::cout << "Test constructor.\n";
    }

    Test(const Test& p_rhs) {
      std::cout << "Test copy constructor.\n";
      temp_ = p_rhs.temp_;
    }

    Test& operator=(const Test& p_rhs) {
      std::cout << "Test copy assignment.\n";

      if(this == &p_rhs)
        return *this;
      
      temp_ = p_rhs.temp_;
      return *this;
    }
    
    ~Test() {
      std::cout << "Test destructor.\n";
    }
    int temp_  = 0;
  };

  std::ostream& operator<<(std::ostream& p_os, const Test& p_other)
  {
    p_os << "Test:" << p_other.temp_;
    return p_os;
  };


template<typename T>
void print(sp::vector<T>& p_vec)
{
  std::cout << "printing vec of size " << p_vec.size() << " and capacity " 
    << p_vec.capacity()  << std::endl;
  for(size_t i = 0; i <= p_vec.size(); ++i)
  {
    try {
      std::cout << p_vec.at(i) << " ";
    }
    catch (std::out_of_range& p_exp) {
      std::cout << " at failed, Error: " << p_exp.what() << '\n';
    }
  }
  std::cout << ".\n";
}



int main()
{
  sp::vector<int> nums;

  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  print(nums);

  
  sp::vector<int> nums10(10);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  print(nums10);

  sp::vector<Test> tests;
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  
}
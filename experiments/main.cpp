#include "vector.hpp"



  struct Test {
    Test() {
      std::cout << "Test() constructor.\n";
      temp_ = new int(0);
    }
    
    ~Test() {
      std::cout << "~Test() destructor.\n";
      delete temp_;
    }

    Test(const Test& p_rhs) {
      std::cout << "Test(const Test& p_rhs) copy constructor.\n";
      temp_ = new int(*p_rhs.temp_);
    }

    Test(Test&& p_rhs) {
      std::cout << "Test(Test&& p_rhs) move constructor.\n";
      temp_ = p_rhs.temp_;
      p_rhs.temp_ = nullptr;
    }

    Test& operator=(const Test& p_rhs) {
      std::cout << "Test=() copy assignment.\n";

      if(this == &p_rhs)
        return *this;
      
      temp_ = new int(*p_rhs.temp_);
      return *this;
    }

    Test& operator=(Test&& p_rhs) {
      std::cout << "Test=() copy assignment.\n";

      if(this == &p_rhs)
        return *this;
      
      temp_ = p_rhs.temp_;
      p_rhs.temp_ = nullptr;

      return *this;
    }
    
    int* temp_  = nullptr;
  };

  std::ostream& operator<<(std::ostream& p_os, const Test& p_other)
  {
    p_os << "Test:" << *p_other.temp_;
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



/*int main()
{
  std::cout << "======== vector default construction and push :================\n";
  sp::vector<int> nums;
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  nums.push_back(0);
  print(nums);
  std::cout << "======== vector default construction and push Ends.================\n\n";


  std::cout << "======== vector with capacity construction and push:================\n";
  sp::vector<int> nums10(10);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  nums10.push_back(0);
  print(nums10);
  std::cout << "======== vector with capacity construction and push End.================\n\n";

  std::cout << "======== vector<Test> construction and push:================\n";
  sp::vector<Test> tests;
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  tests.push_back(Test());
  std::cout << "======== vector<Test> construction and push end.================\n\n";


  std::cout << "vector copy construction:\n";
  auto tests2 = tests;
  std::cout << "vector copy construction end.\n\n";

  std::cout << "vector copy assigment:\n";
  sp::vector<Test> tests3;
  tests3 = tests2;
  std::cout << "vector copy assigment end.\n\n";

  std::cout << "vector move construtor:\n";
  sp::vector<Test> tests4 = std::move(tests3);
  std::cout << "vector move construtor end.\n\n";

  std::cout << "vector move assignment:\n";
  sp::vector<Test> tests5;
  tests5 = std::move(tests4);
  std::cout << "vector move assignment end.\n";
}*/
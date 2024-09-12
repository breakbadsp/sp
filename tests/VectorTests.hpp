#pragma once
#include "vector.hpp"
struct Temp {
    Temp() = default;
    ~Temp() { delete ptr; }

    Temp(const Temp& p_rhs) { 
        if(ptr)
            ptr = new int(*p_rhs.ptr);
    }

    Temp(Temp&& p_rhs)
    : ptr(p_rhs.ptr) {
        p_rhs.ptr = nullptr;
    }

    Temp& operator=(const Temp& p_rhs)
    {
        if(this != &p_rhs) {
            delete ptr;
            ptr = new int(*p_rhs.ptr);
        }
        return *this;
    }

    Temp& operator=(Temp&& p_rhs) {
        if(this != &p_rhs) {
            ptr = p_rhs.ptr;
            p_rhs.ptr = nullptr;
        }
        return *this;
    }

    Temp(int p)
    {
        ptr = new int(p);
    } 


    int* ptr = nullptr;

    friend std::ostream& operator<<(std::ostream& p_os, const Temp& p_rhs) {
        if(p_rhs.ptr)
            p_os << *p_rhs.ptr;
        return p_os;
    }
};


void print(sp::vector<Temp>& p_elems) {
    for(size_t i = 0; i < p_elems.size(); ++i)
        std::cout << p_elems[i] << ' ';
    
    std::cout << ": Printed vector \n";
}

void print(sp::vector<int>& p_elems) {
    for(size_t i = 0; i < p_elems.size(); ++i)
        std::cout << p_elems[i] << ' ';
    
    std::cout << ": Printed vector \n";
}

void Test1()
{
    sp::vector<int> temp1;
    print(temp1);

    sp::vector<int> temp1_5(5);
    temp1_5.push_back(1);
    temp1_5.push_back(2);
    temp1_5.push_back(3);
    temp1_5.push_back(4);
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);
    print(temp1_5);
    auto ele = temp1_5.pop_back();
    print(temp1_5);
    std::cout << ele << '\n';
    

    sp::vector<int> temp2(temp1_5);
    print(temp2);
    sp::vector<int> temp4(std::move(temp1_5)); 
    print(temp4);

    sp::vector<int> temp3;
    temp3 = temp2;
    print(temp3);
    sp::vector<int> temp5;
    temp5 = std::move(temp2);    
    print(temp5);
}
void Test2()
{
    sp::vector<Temp> temp1;
    print(temp1);

    sp::vector<Temp> temp1_5(5);
    temp1_5.push_back(1);
    temp1_5.push_back(2);
    temp1_5.push_back(3);
    temp1_5.push_back(4);
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);

    Temp t1, t2, t3;
    temp1_5.push_back(t1);
    temp1_5.push_back(t2);
    temp1_5.push_back(t3);


    print(temp1_5);
    auto ele = temp1_5.pop_back();
    print(temp1_5);
    std::cout << ele << '\n';
    

    sp::vector<Temp> temp2(temp1_5);
    print(temp2);
    sp::vector<Temp> temp4(std::move(temp1_5)); 
    print(temp4);

    sp::vector<Temp> temp3;
    temp3 = temp2;
    print(temp3);
    sp::vector<Temp> temp5;
    temp5 = std::move(temp2);    
    print(temp5);
}

void Test3()
{
    sp::vector<Temp> temp1;
    print(temp1);

    sp::vector<Temp> temp1_5(5);
    temp1_5.push_back(1);
    temp1_5.push_back(2);
    temp1_5.push_back(3);
    temp1_5.push_back(4);
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);
    
    Temp t1, t2, t3;
    temp1_5.push_back(t1);
    temp1_5.push_back(t2);
    temp1_5.push_back(t3);
    
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);
    temp1_5.push_back(10);
    temp1_5.push_back(11);
    temp1_5.push_back(12);
    
    print(temp1_5);
    auto ele = temp1_5.pop_back();
    print(temp1_5);
    std::cout << ele << '\n';
    

    sp::vector<Temp> temp2(temp1_5);
    print(temp2);
    sp::vector<Temp> temp4(std::move(temp1_5)); 
    print(temp4);

    sp::vector<Temp> temp3;
    temp3 = temp2;
    print(temp3);
    sp::vector<Temp> temp5;
    temp5 = std::move(temp2);    
    print(temp5);
}

void TestVectorPushBack()
{
  std::cout << "Pushback test\n";
  sp::vector<int> test_vec(1);
  print(test_vec);
  test_vec.push_back(2);
  test_vec.push_back(3);
  test_vec.push_back(4);
  print(test_vec);
  std::cout << test_vec.back() << '\n';
  test_vec.pop_back();
  print(test_vec);
}

void TestVector()
{
  sp::vector<int> test_vec;
  print(test_vec);
  Test1();
  Test2();
  Test3();
  TestVectorPushBack();
}

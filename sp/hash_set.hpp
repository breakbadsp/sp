#pragma once

#include <type_traits>
#include <vector>

#include "Hashing.hpp"

namespace sp
{

template <class T>
class has_set
{
public:

  bool insert(T p_key)
  {
    size_t index = 0;
    if(!find(p_key, index))
    {
      std::cout << "inserting at " << index << '\n';
      table_[index].state_ = Bucket::State::Occupied;
      table_[index].data_ = p_key;
      return true;
    }
    return false;
  }

  bool erase(T&& p_key)
  {
    size_t index = 0;
    if(find(p_key, index))
    {
      table_[index].state_ = Bucket::State::Tumbstone;
      return true;
    }
    return false;
  }

  auto find(T p_key)
  {
    size_t index = 0;
    return find(p_key, index);
  }

  auto find(T p_key, size_t& p_index)
  {
    auto index = Hash(p_key, table_.capacity());
    p_index = index;
    const auto orig_index = index;
    while(index >= 0 && index < table_.capacity())
    {
      auto& bucket = table_[index];
      if(bucket.state_ == Bucket::State::Unused)
        return false; //not found
      
      if(orig_index == index + 1 )
        break;//unlikely but safe

      if(bucket.state_ == Bucket::State::Occupied) 
      {
        if(bucket.data_ == p_key)
        {
          std::cout << "Element " << p_key << " found at index " << index << '\n';
          p_index = index;
          return true;
        }

        index = (index+1) % table_.capacity();
        continue;
      }

      if(bucket.state_ == Bucket::State::Tumbstone)
      {
        ++index;
        continue;
      }

    }
    return false;
  }

  void Print()
  {
    std::cout << "Hash Set: \n";

    for(const auto& ele: table_)
    {
      std::cout << (int)ele.state_ << ":" << ele.data_ << ", ";
    }
    std::cout << '\n';
  }

private:
  struct Bucket
  {
    enum State 
    {
      Unused = 1
      , Occupied
      , Tumbstone
      , Count
    };

    T data_;
    State state_ {State::Unused};
  };

  std::vector<Bucket> table_ {10};//TODO::replace this with sp::vector

};
}//namespace sp
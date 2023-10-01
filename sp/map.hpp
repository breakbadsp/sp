#pragma once
#include <iostream>

#include "shared_ptr.hpp"

namespace sp 
{
template<typename KeyType, typename ValueType>
class map final 
{
  public:
    struct Node 
    {
      Node(const std::pair<KeyType, ValueType>& p_pair) 
      {
        key_ = p_pair.first;
        value_ = p_pair.second;
      }

      bool Insert(const std::pair<KeyType, ValueType>& p_pair) 
      {
        if(p_pair.first < key_) 
        {
          if(left() == nullptr) 
          {
            left_.reset(new Node(p_pair));
            std::cout << "inserted " << p_pair.first << " on left of " << key_ << "\n";
            return true;
          }
          return left_->Insert(p_pair);
        }
          
        if(p_pair.first > key_) 
        {
          if(right() == nullptr) 
          {
            right_.reset(new Node(p_pair));
            std::cout << "inserted " << p_pair.first << " on right of " << key_ << "\n";
            return true;
          }
          return right_->Insert(p_pair);
        }
    
        std::cout << "insert failed " << p_pair.first << " is not unique\n";
        return false;
      }

      
      bool IsLeafNode() const 
      {
        return ( (left() == nullptr) && (right() == nullptr));
      }

      //TODO:: this should be const member function
      void  PrintInOrder() 
      {
        if(IsLeafNode()) 
        {
          std::cout << key() << ':' << value() << ' ';
          return;
        }
      
      if(left_) left_->PrintInOrder();
      std::cout << key() << ':' << value() << ", "; 
      if(right_) right_->PrintInOrder();
    }

      //getters
      const KeyType& key() const { return key_; }
      const ValueType& value() const { return value_; }
      const sp::shared_ptr<Node>& left() const { return left_; }
      const sp::shared_ptr<Node>& right() const { return right_; }

      KeyType key_;
      ValueType value_;
      sp::shared_ptr<Node> left_;
      sp::shared_ptr<Node> right_;
    }; //Node

    map() = default;
    ~map() = default;

    bool insert(const std::pair<KeyType, ValueType>& p_pair) 
    {
      if(Empty()) 
      {
        root_.reset(new Node(p_pair));
        std::cout << "inserted " << p_pair.first << " at root\n";
        return true;
      }
      return root_->Insert(p_pair);
    }

    bool Empty() const 
    {
      if(!root_)
        return true;
      
      return false;
    }

    //TODO:: this should be const member function
    void Print() 
    {
      if(Empty())
      {
        std::cout << "Nothing to print in this sp::map\n";
        return;
      }
      root_->PrintInOrder();
    }

  private:
    sp::shared_ptr<Node> root_;

}; //map
} //namesapce sp
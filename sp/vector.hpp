#pragma once

#include <memory>
#include <iostream>
#include <cassert>

namespace sp{
template<typename T>
class vector
{
  public:
    ~vector()
    {
      Clear();
    }

    //getters
    size_t size() { return size_; }
    size_t capacity() { return capacity_; }

    void Print()
    {
      std::cout << std::endl;
      std::cout << "Printing vector of size=" << size_ << '\n';
      for(size_t i = 0; i < size_; ++i)
        std::cout << buffer_[i] << ", ";
      std::cout << std::endl;
    }

    void push_back(const T& p_data)
    {
      if(size_ == capacity_)
      {
        Reallocate();
      }
      buffer_[size_++] = p_data;
    }

    void pop_back()
    {
      if(size_ <= 0)
        return;
      --size_;
    }

    const T& back()
    {
      assert(size_ > 0);
      return buffer_[size_ - 1];
    }

    vector() : size_(0) ,capacity_(1)
    {
      buffer_  = new T[capacity_];
    }

    vector(const T& p_data) : size_(0), capacity_(1)
    {
      buffer_ = new T[capacity_];
      buffer_[size_++] = p_data;
    }

    vector(const vector<T>& p_other) 
    : size_(p_other.size_)
      ,capacity_(p_other.capacity_)
    {
      for(size_t i = 0; i < capacity_; ++i)
      {
        buffer_[i] = new T(*p_other.buffer[i]);
      }
    }

    vector(vector<T>&& p_other) 
    : size_(std::move(p_other.size_))
      ,capacity_(std::move(p_other.capacity_))
    {
      //TODO:: Add ForEach member function for this class
      for(size_t i = 0; i < capacity_; ++i)
      {
        buffer_[i] = new T(std::move(*p_other.buffer[i]));
      } 
    }

  private:
    void Clear()
    {
      delete [] buffer_;
      buffer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }

    void Reallocate()
    {
      auto* old_buffer = buffer_;
      auto old_capacity = capacity_;
      
      capacity_ *= 2;
      buffer_ = new T[capacity_];
      
      for (size_t i = 0; i < old_capacity; ++i)
      {
        buffer_[i] = old_buffer[i];
      }
      delete [] old_buffer;
    }

  private:
    size_t size_;
    size_t capacity_;
    //TODO:: Add inline buffer for small vector size.
    //TODO:: convert this into a smart pointer, preferably uniqueu pointer
    T* buffer_;

};//vector class
};//sp namespace
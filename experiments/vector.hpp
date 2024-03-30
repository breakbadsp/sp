#include <iostream>
// vector
// rule of 5 here
//
// push_back(T data)
// T pop_back()
// 
// data 
// buffer_ => the real data storage on heap
// size_ = > number elements present
// capacity_ => total available space on heap
// 
// size_ > capacity_ => reallocate the vector
// reallocate(new_size)


namespace sp 
{
template<typename T>
class vector 
{
  public:
    vector() = default;
    vector(size_t p_capacity) 
    : size_(0)
    , capacity_(p_capacity)
    {
      buffer_ = new T[p_capacity];
    }

    ~vector()
    {
      delete [] buffer_;
    }

    vector(const vector& p_rhs)
    : size_ (p_rhs.size_)
    , capacity_(p_rhs.capacity_)
    {
      buffer_ = new T[p_rhs.capacity_];
      for(size_t i = 0; i < p_rhs.size_; ++i) {
        buffer_[i] = p_rhs.buffer_[i];
      }
    }

    vector(vector&& p_rhs)
    : size_(std::move(p_rhs.size_))
    , capacity_(std::move(p_rhs.capacity_))
    , buffer_(p_rhs.buffer_)
    {
      p_rhs.buffer_ = nullptr;
      p_rhs.size_ = 0;
      p_rhs.capacity_ = 0;
    }

    vector& operator=(const vector& p_rhs)
    {
      if(this == &p_rhs)
        return *this;

      auto* new_buffer = new T[p_rhs.capacity_];
      for(size_t i = 0; i < p_rhs.size_; ++i)
      {
        new_buffer[i] = p_rhs.buffer_[i];
      }

      delete [] buffer_;
      buffer_ = new_buffer;
      size_ = p_rhs.size_;
      capacity_ = p_rhs.capacity_;
    }

    vector& operator=(vector&& p_rhs)
    {
      if(this == &p_rhs)
        return *this;

      delete [] buffer_;
      buffer_ = p_rhs.buffer_;
      size_ = p_rhs.size_;
      capacity_ = p_rhs.capacity_;

      p_rhs.buffer_ = nullptr;
      p_rhs.size_ = 0;
      p_rhs.capacity_ = 0;
    }

    void push_back(const T& p_data)
    {
      if(size_ >= capacity_)
      {
        reallocate(capacity_ == 0 ? 4 : capacity_ *  2);
      }

      buffer_[size_++] = p_data;
      std::cout << "pushed " << p_data << " at " << size_ - 1 << '\n';
    }

    T pop_back()
    {
      return buffer_[--size_];
    }

    T& at(size_t p_index)
    {
      if(p_index >= size_)
      {
        throw std::out_of_range {
          "out of range"
        };
      }

      return buffer_[p_index];
    }

    //getters
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

  private:
    T* buffer_ = {nullptr};
    size_t size_ = 0;
    size_t capacity_ = 0;


    void reallocate(size_t p_new_size)
    {
      std::cout << "reallocating from " << capacity_ << " to " << p_new_size << '\n';
      auto* new_buffer = new T[p_new_size];
      for(size_t i = 0; i < size_; ++i)
      {
        new_buffer[i] = buffer_[i];
      }
      delete [] buffer_;
      buffer_ = new_buffer;
      capacity_ = p_new_size;
    }
};
}



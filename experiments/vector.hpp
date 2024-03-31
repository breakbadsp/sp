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
    //special member funcs
    vector() = default;
    
    vector(size_t p_capacity) 
    : size_(0)
    , capacity_(0)
    {
      reallocate(p_capacity);
    }

    ~vector()
    {
      clear();
    }

    vector(const vector& p_rhs)
    : size_ (p_rhs.size_)
    , capacity_(p_rhs.capacity_)
    {
      buffer_ = new T[p_rhs.capacity_];
      for(size_t i = 0; i < p_rhs.size_; ++i) {
        reinterpret_cast<T*>(buffer_)[i] = reinterpret_cast<T*>(p_rhs.buffer_)[i];
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
        reinterpret_cast<T*>(new_buffer)[i] = reinterpret_cast<T*>(p_rhs.buffer_)[i];
      }

      clear();
      buffer_ = new_buffer;
      size_ = p_rhs.size_;
      capacity_ = p_rhs.capacity_;
    }

    vector& operator=(vector&& p_rhs)
    {
      if(this == &p_rhs)
        return *this;

      clear();
      buffer_ = p_rhs.buffer_;
      size_ = p_rhs.size_;
      capacity_ = p_rhs.capacity_;

      p_rhs.buffer_ = nullptr;
      p_rhs.size_ = 0;
      p_rhs.capacity_ = 0;
    }

    // methods/apis
    void push_back(const T& p_data)
    {
      if(size_ >= capacity_)
      {
        reallocate(capacity_ == 0 ? 4 : capacity_ *  2);
      }

      new (buffer_ + ((size_) * sizeof(T))) T(p_data);
      ++size_;
      std::cout << "pushed " << p_data << " at " << size_ - 1 << '\n';
    }

    T pop_back()
    {
      return reinterpret_cast<T*>(buffer_)[--size_];
    }

    T& at(size_t p_index)
    {
      if(p_index >= size_)
      {
        std::string error = "out of range index ";
        error += std::to_string(p_index);
        throw std::out_of_range {
          error
        };
        //Or we can return std::optional
        //or create our own type sp::option<T>
      }

      return reinterpret_cast<T*>(buffer_)[p_index];
    }

    //getters
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

  private:
    //data members
    alignas(T) unsigned char* buffer_ = {nullptr};
    size_t size_ = 0;
    size_t capacity_ = 0;



  private:
    //utility functions:
    void reallocate(size_t p_new_capacity)
    {
      std::cout << "reallocating from " << capacity_ << " to " << p_new_capacity << '\n';
      alignas(T) unsigned char* new_buffer = reinterpret_cast<unsigned char*>(std::malloc(p_new_capacity * sizeof(T)));
      if(!new_buffer)
      {
        std::cout << "failed to allocate memory\n";
        return;
      }
      
      if(buffer_)
      {
        for(size_t i = 0; i < size_; ++i)
        {
          reinterpret_cast<T*>(new_buffer)[i] = reinterpret_cast<T*>(buffer_)[i];
        }
        free(buffer_);
      }

      buffer_ = new_buffer;
      capacity_ = p_new_capacity;
    }

    void clear()
    {
      free(buffer_);
      size_ = 0;
      capacity_ = 0;
      buffer_ = nullptr;
    }
};
}



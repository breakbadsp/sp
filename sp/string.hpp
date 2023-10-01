#pragma once

#include <cstring>
#include <iostream>

#include "owning_ptr.hpp"

namespace sp
{

class string
{

  public:
    string() = default;

    ~string() { delete [] buffer_; }
    
    string(const char* p_c_style) 
    {
      Init(p_c_style);
    }

    explicit string(decltype(nullptr))
    {
      string();
    }

    string(const string& p_other)
    {
      Init(p_other.buffer_);
    }

    explicit string(string&& p_other) :
      buffer_(sp::move(p_other.buffer_))
      ,size_(sp::move(p_other.size_))
      ,capacity_(sp::move(p_other.capacity_))
    {
      p_other.buffer_ = nullptr;
      p_other.size_ = 0;
      p_other.capacity_= 0;
    }

    string& operator=(const string& p_other)
    {
      if(this == &p_other)
        return *this;

      Init(p_other.buffer_);
      return *this;
    }

    string& operator=(string&& p_other)
    {
      if(this == &p_other)
        return *this;
      
      buffer_ = sp::move(p_other.buffer_);
      size_ = sp::move(p_other.size_);
      capacity_ = sp::move(p_other.capacity_);

      p_other.buffer_ = nullptr;
      p_other.size_ = 0;
      p_other.capacity_ = 0;
      return *this;
    }

    string& operator=(const char* p_c_style) {
      Init(p_c_style);
      return *this;
    }

    string operator+(const string& p_other) const {
      string new_string;
      new_string.Init(get_capacity() + p_other.capacity_);
      char* new_buffer_ptr = new_string.buffer_;
      strcpy(new_buffer_ptr, get_raw_buffer());
      strcpy(new_buffer_ptr + get_capacity(), p_other.buffer_);
      return new_string;
    }

    string& operator+=(const string& p_other){
      if(p_other.capacity_ <= 0)
        return *this;

      const size_t new_size = capacity_ + p_other.capacity_ + 1;
      auto* new_buffer_ptr = new char[new_size];
      strcpy(new_buffer_ptr, buffer_); //FIXME:: heap-buffer-overflow
      strcpy(new_buffer_ptr + sizeof(buffer_) - 1, p_other.buffer_);
      delete [] buffer_;
      buffer_ = new_buffer_ptr;
      return *this;
    }

    friend std::ostream& operator<< (std::ostream& p_os, const sp::string& p_string){
      p_os << p_string.get_raw_buffer();
      return p_os;
    }

    //getters
    const char* get_raw_buffer() const { return buffer_; }
    const unsigned int& get_size() const { return size_; }
    const unsigned int& get_capacity() const { return capacity_; }


  private:
    char* buffer_ {nullptr};
    unsigned int size_ {0};
    unsigned int capacity_ {0};

    void Init(const char* p_c_style)
    {
      Init(strlen(p_c_style)+1);
      strncpy(buffer_, p_c_style, size_);
    }

    void Init(const unsigned p_len)
    {
      buffer_ = new char[p_len];
      size_ = p_len;
      capacity_ = p_len;
    }

};
} //sp
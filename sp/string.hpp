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
    {
      p_other.buffer_ = nullptr;
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

      p_other.buffer_ = nullptr;
      return *this;
    }

    string& operator=(const char* p_c_style) {
      Init(p_c_style);
      return *this;
    }

    string operator+(const string& p_other) const {

      size_t new_len = p_other.get_size() + get_size() + 1;
      string new_string;
      new_string.buffer_ = new char[new_len];

      char* new_buffer_ptr = new_string.buffer_;
      strncpy(new_buffer_ptr, get_raw_buffer(), get_size());
      strcpy(new_buffer_ptr + get_size(), p_other.buffer_);
      return new_string;
    }

    string& operator+=(const string& p_other){
      if(p_other.get_size() <= 0)
        return *this;

      const size_t new_size = get_size() + p_other.get_size() + 1;
      auto* new_buffer_ptr = new char[new_size];
      strncpy(new_buffer_ptr, buffer_, get_size());
      strcpy(new_buffer_ptr + get_size(), p_other.buffer_);
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
    unsigned int get_size() const { return strlen(buffer_); }


  private:
    char* buffer_ {nullptr};

    void Init(const char* p_c_style)
    {
      size_t len = strlen(p_c_style) + 1;
      buffer_ = new char[len];
      strcpy(buffer_, p_c_style);
    }
};
} //sp
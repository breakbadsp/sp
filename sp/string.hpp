#pragma once

#include <cstring>
#include <iostream>

#include "owning_ptr.hpp"

namespace sp
{

class string
{

  public:
    constexpr string() = default;
    ~string() { 
      delete [] buffer_; 
    }
    
    explicit string(const char* p_c_style) {
      Init(p_c_style);
    }

    //TODO:: Add support for char arrays of known size or null terminated arrays
    explicit string(char p_null_term_array[]) {
      Init(p_null_term_array);
    }

    explicit string(const string& p_other) {
      Init(p_other.buffer_);
    }

    string(string&& p_other) noexcept
    : buffer_(sp::move(p_other.buffer_))
    , size_(sp::move(p_other.size_)) {
      p_other.buffer_ = nullptr;
      p_other.size_ = 0;
    }

    string& operator=(const string& p_other) {
      if(this == &p_other)
        return *this;

      Init(p_other.buffer_);
      return *this;
    }

    string& operator=(string&& p_other) noexcept {
      if(this == &p_other)
        return *this;
      
      buffer_ = sp::move(p_other.buffer_);
      size_ = sp::move(p_other.size_);

      p_other.buffer_ = nullptr;
      p_other.size_ = 0;
      return *this;
    }

    string operator+(const string& p_other) const {

      size_t new_len = p_other.get_size() + get_size() + 1;
      string new_string;
      new_string.buffer_ = new char[new_len];
      new_string.size_ = new_len - 1;

      char* new_buffer_ptr = new_string.buffer_;
      strncpy(new_buffer_ptr, c_str(), get_size());
      strcpy(new_buffer_ptr + get_size(), p_other.buffer_);

      return new_string;
    }

    string& operator+=(const string& p_other) {
      if(p_other.get_size() <= 0)
        return *this;

      if(get_size() <= 0) {
        *this = p_other;
        return *this;
      }

      const size_t new_size = get_size() + p_other.get_size() + 1;
      auto* new_buffer = new char[new_size];
      strcpy(new_buffer, buffer_);
      strcpy(new_buffer + get_size(), p_other.buffer_);
      delete [] buffer_;
      buffer_ = new_buffer;
      size_ = new_size - 1;
      return *this;
    }

    friend std::ostream& operator<< (std::ostream& p_os, const sp::string& p_string){
      if(p_string.c_str())
        p_os << p_string.c_str();
      return p_os;
    }

    //getters
    const char* c_str() const noexcept { return buffer_; }
    size_t get_size() const noexcept { return size_; }


  private:
    char* buffer_ {nullptr};
    size_t size_ {0};

    void Init(const char* p_c_style) {
      delete [] buffer_;  // Add this line
      if (!p_c_style) {
          buffer_ = nullptr;
          size_ = 0;
          return;
      }
      size_t len = strlen(p_c_style) + 1;
      buffer_ = new char[len];
      size_ = len - 1;
      strcpy(buffer_, p_c_style);
    }
};
} //sp
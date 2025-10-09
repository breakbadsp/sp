#pragma once

#include <cstring>
#include <iostream>

#include "owning_ptr.hpp"

namespace sp
{

class string
{

  public:
    constexpr string() noexcept = default;
    ~string() noexcept { 
      delete [] buffer_; 
    }
    
    explicit string(const char* p_c_style) {
      Init(p_c_style);
    }

    //TODO:: Add support for char arrays of known size or null terminated arrays
    explicit string(char p_null_term_array[]) {
      Init(p_null_term_array);
    }

    string(const string& p_other) {
      Init(p_other.buffer_);
    }

    string(string&& p_other) noexcept
    : buffer_(sp::move(p_other.buffer_))
    , size_(sp::move(p_other.size_)) {

      p_other.buffer_ = nullptr;
      p_other.size_ = 0;
    }
 
    //copy and swap idiom
    string& operator=(string p_other) noexcept {  // parameter is passed by value, so copy happens before this function
      swap(p_other);
      return *this;
    }

    // We do not need move assignment operator as we have copy and swap idiom implemented
    // Copy and swap idiom will do move and swap when rvalue is passed
    // that is very neat and clean implementation
    
    string operator+(const string& p_other) const {

      size_t new_len = p_other.get_size() + get_size() + 1;
      string new_string;
      new_string.buffer_ = new char[new_len];
      new_string.size_ = new_len - 1;

      char* new_buffer = new_string.buffer_;
      memcpy(new_buffer, c_str(), get_size());
      memcpy(new_buffer + get_size(), p_other.buffer_, p_other.get_size() + 1);

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
      memcpy(new_buffer, buffer_, get_size());
      memcpy(new_buffer + get_size(), p_other.buffer_, p_other.get_size() + 1);
      delete [] buffer_;
      buffer_ = new_buffer;
      size_ = new_size - 1;
      return *this;
    }

    //friend functions
    friend std::ostream& operator<< (std::ostream& p_os, const sp::string& p_string){
      if(p_string.c_str())
        p_os << p_string.c_str();
      return p_os;
    }

    friend void swap(string& p_lhs, string& p_rhs) noexcept {
      using std::swap;
      swap(p_lhs.buffer_, p_rhs.buffer_);
      swap(p_lhs.size_, p_rhs.size_);
    }

    void swap(string& p_other) noexcept {
      using std::swap;
      swap(buffer_, p_other.buffer_);
      swap(size_, p_other.size_);
    }

    bool operator==(const string& p_other) const noexcept {
      if (size_ != p_other.size_)
        return false;
      return strcmp(buffer_, p_other.buffer_) == 0;
    }
    
    bool operator!=(const string& p_other) const noexcept {
        return !(*this == p_other);
    }
    
    bool operator<(const string& p_other) const noexcept {
        return strcmp(buffer_, p_other.buffer_) < 0;
    }
    
    bool operator>(const string& p_other) const noexcept {
        return p_other < *this;
    }
    
    bool operator<=(const string& p_other) const noexcept {
        return !(p_other < *this);
    }
    
    bool operator>=(const string& p_other) const noexcept {
        return !(*this < p_other);
    }

    void clear() noexcept {
      delete[] buffer_;
      buffer_ = nullptr;
      size_ = 0;
    }

    static constexpr size_t npos = static_cast<size_t>(-1);

    string substr(size_t pos = 0, size_t len = npos) const {
      if (pos > size_)
        throw std::out_of_range("Position out of range");

      const size_t actual_len = (len == npos || pos + len > size_) ? size_ - pos : len;
      string result;
      if (actual_len > 0) {
        result.buffer_ = new char[actual_len + 1];
        strncpy(result.buffer_, buffer_ + pos, actual_len);
        result.buffer_[actual_len] = '\0';
        result.size_ = actual_len;
      }
      return result;
    }

    size_t find(const string &str, size_t pos = 0) const noexcept {
      if (pos >= size_ || str.is_empty() || str.get_size() > size_)
        return npos;
      
      return find(str.c_str(), pos);
    }

    size_t find(const char *s, size_t pos = 0) const {
      if (!s)
        throw std::invalid_argument("Null pointer passed to find");

      if (pos >= size_ || *s == '\0')
        return npos;

    const char *found = strstr(buffer_ + pos, s);
      if (!found)
        return npos;

      return found - buffer_;
    }

    //getters
    const char* c_str() const noexcept { return buffer_; }
    size_t get_size() const noexcept { return size_; }

    bool is_empty() const noexcept { return size_ == 0; }
    

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
      size_ = strlen(p_c_style);
      buffer_ = new char[size_ + 1];
      memcpy(buffer_, p_c_style, size_ + 1); // including null terminator
    }
};
} //sp
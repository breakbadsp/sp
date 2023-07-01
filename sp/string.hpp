#include <cstring>
#include <iostream>

#include "owning_ptr.hpp"

namespace sp {

class string {

  public:
    string() = default;

    ~string() { delete [] buffer_; }
    
    string(const char* p_c_style) {
      Init(p_c_style);
    }

    explicit string(decltype(nullptr)) {
      string();
    }

    string(const string& p_other) {
      Init(p_other.buffer_);
    }

    explicit string(string&& p_other) :
      buffer_(std::move(p_other.buffer_))
      ,size_(std::move(p_other.size_))
      ,capacity_(std::move(p_other.capacity_))
    {
      p_other.buffer_ = (nullptr);
      p_other.size_ = (0);
      p_other.capacity_= (0);
    }

    string& operator=(const string& p_other) {
      if(this == &p_other)
        return *this;

      Init(p_other.buffer_);
      return *this;
    }

    string& operator=(string&& p_other) {
      if(this == &p_other)
        return *this;
      
      buffer_ = std::move(p_other.buffer_);
      size_ = std::move(p_other.size_);
      capacity_ = std::move(p_other.capacity_);
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

      auto* new_buffer_ptr = new char[capacity_ + p_other.capacity_];
      strcpy(new_buffer_ptr, buffer_);
      strcpy(new_buffer_ptr + capacity_, p_other.buffer_);
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

    void Init(const char* p_c_style) {
      Init(strlen(p_c_style));
      strcpy(buffer_, p_c_style);
    }

    void Init(const unsigned p_len) {
      buffer_ = new char[p_len + 1];
      size_ = p_len;
      capacity_ = p_len;
    }

};
} //sp
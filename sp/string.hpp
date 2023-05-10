#include <cstring>

#include "sp/owning_ptr.hpp"

namespace sp {

class string {

  public:
    string() = default;
    
    explicit string(const char* p_c_style) {
      Init(p_c_style);
    }

    explicit string(decltype(nullptr)){
      string();
    }

    explicit string(const string& p_other) {
      Init(p_other.buffer_.get());
    }

    explicit string(string&& p_other) :
      buffer_(std::move(p_other.buffer_))
      ,size_(std::move(p_other.size_))
      ,capacity_(std::move(p_other.capacity_))
    {}

    string& operator=(const string& p_other) {
      if(this == &p_other)
        return *this;

      Init(p_other.buffer_.get());
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

  private:
    sp::owning_ptr<char> buffer_ {nullptr};
    unsigned int size_ {0};
    unsigned int capacity_ {0};

    void Init(const char* p_c_style) {
      const auto len = strlen(p_c_style); //UB if p_c_style is not null terminated
      buffer_.reset(new char[len + 1]);
      size_ = len;
      capacity_ = len;
      strcpy(buffer_.get(), p_c_style);
    }
};
} //sp
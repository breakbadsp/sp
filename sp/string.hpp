#include <cstring>

#include "sp/owning_ptr.hpp"

namespace sp {

class string {

  public:
    string() = default;
    
    string(char* p_c_style) {
      const auto len = strlen(p_c_style);
      sp::owning_ptr<char> buffer( new char[len + 1] );
      size_ = len + 1;
      capacity_ = len + 1;
      strcpy(buffer_.get(), p_c_style);
    }



  private:
    sp::owning_ptr<char> buffer_ {nullptr};
    unsigned int size_ {0};
    unsigned int capacity_ {0};
};
} //sp
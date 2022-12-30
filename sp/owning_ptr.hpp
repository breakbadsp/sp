#include <utility>

namespace sp{

template<typename T>
class owning_ptr final {

  public:
    owning_ptr() = default;

    ~owning_ptr() {
      delete raw_ptr_;
      raw_ptr_ = nullptr;
    }
    
    
    owning_ptr(T* p_raw) : raw_ptr_(p_raw) {}
    
    //Delete copy
    owning_ptr(const owning_ptr&) = delete;
    owning_ptr& operator=(const owning_ptr&) = delete;
    const owning_ptr& operator=(const owning_ptr&) const = delete;

    //Moves
    owning_ptr(owning_ptr&& p_other) {
      
      if(&p_other == this)
        return;

      raw_ptr_ = std::move(p_other.raw_ptr_);
      p_other.raw_ptr_ = nullptr;
    }

    owning_ptr& operator=(owning_ptr&& p_other){
      
      if(&p_other == this)
        return *this;

      raw_ptr_ = std::move(p_other.raw_ptr_);
      p_other.raw_ptr_ = nullptr;

      return *this;
    }

    owning_ptr& operator=(T* p_raw) { 
      raw_ptr_ = p_raw; 
      return *this;
    }

    T operator[](size_t p_index) { return raw_ptr_[p_index]; }

    void reset(owning_ptr&& p_other) {
      delete raw_ptr_;
      raw_ptr_ = std::move(p_other.raw_ptr_);
      p_other.raw_ptr_ = nullptr;
    }

    const T* get() const { return raw_ptr_; }
    T* get() { return raw_ptr_; }

    //with raw
    bool operator==(T* p_raw) { return p_raw == raw_ptr_; }
    bool operator!=(T* p_raw) { return p_raw != raw_ptr_; }

    bool operator==(std::nullptr_t p_null) { return p_null == raw_ptr_; }
    bool operator!=(std::nullptr_t p_null) { return p_null != raw_ptr_; }

    bool operator==(const owning_ptr& p_other) { return raw_ptr_ == p_other.raw_ptr_; }
    bool operator!=(const owning_ptr& p_other) { return raw_ptr_ != p_other.raw_ptr_; }

    //TODO::Overload an arrow -> operator
    //T* operator->() { return raw_ptr_; }

  private:
    T* raw_ptr_ {nullptr};
};

template<typename T, typename U>
sp::owning_ptr<T> make_owning(U&& p_u) {
  return sp::owning_ptr(new T(std::forward<U>(p_u)));
}

///FIXME::ASAN Leak report
template<typename T, typename... Args>
sp::owning_ptr<T> make_owning(Args&&... p_args) {
  return sp::owning_ptr( new T(std::forward<Args>(p_args)...) );  
}

}


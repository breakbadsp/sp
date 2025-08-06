#pragma once
#include <utility>
#include <type_traits>

#include "cmn.hpp"

namespace sp
{

template<typename T>
class owning_ptr final 
{

  public:
    constexpr owning_ptr() = default;

    ~owning_ptr() {
      if(std::is_bounded_array_v<T> || std::is_unbounded_array_v<T>)
        delete [] raw_ptr_;
      else
        delete raw_ptr_;
      raw_ptr_ = nullptr;
    }

    explicit owning_ptr(T* p_raw) : raw_ptr_(p_raw) {}
    
    //Delete copy
    owning_ptr(const owning_ptr&) = delete;
    owning_ptr& operator=(const owning_ptr&) = delete;
    const owning_ptr& operator=(const owning_ptr&) const = delete;

    //Moves
    owning_ptr(owning_ptr&& p_other) noexcept {
      
      if(&p_other == this)
        return;

      raw_ptr_ = std::move(p_other.raw_ptr_);
      p_other.raw_ptr_ = nullptr;
    }

    owning_ptr& operator=(owning_ptr&& p_other) noexcept {
      
      if(&p_other == this)
        return *this;

      raw_ptr_ = std::move(p_other.raw_ptr_);
      p_other.raw_ptr_ = nullptr;

      return *this;
    }

    /*owning_ptr& operator=(T* p_raw)  // this should be compiler error
    { 
      raw_ptr_ = p_raw; 
      return *this;
    }*/

    T operator[](size_t p_index) { return raw_ptr_[p_index]; }

    void reset(owning_ptr&& p_other) noexcept {
      delete raw_ptr_;
      raw_ptr_ = std::move(p_other.raw_ptr_);
      p_other.raw_ptr_ = nullptr;
    }

    void reset(T* p_raw) noexcept {
      delete raw_ptr_;
      raw_ptr_ = p_raw;
    }

    void reset(decltype(nullptr)) noexcept {
      delete raw_ptr_;
    }

    T* release() noexcept {
      auto* temp = raw_ptr_;
      raw_ptr_ = nullptr;
      return temp;
    }

    const T* get() const noexcept { return raw_ptr_; }
    T* get() noexcept { return raw_ptr_; }

    //with raw
    bool operator==(T* p_raw) { return p_raw == raw_ptr_; }
    bool operator!=(T* p_raw) { return p_raw != raw_ptr_; }

    bool operator==(std::nullptr_t p_null) { return p_null == raw_ptr_; }
    bool operator!=(std::nullptr_t p_null) { return p_null != raw_ptr_; }

    bool operator==(const owning_ptr& p_other) { return raw_ptr_ == p_other.raw_ptr_; }
    bool operator!=(const owning_ptr& p_other) { return raw_ptr_ != p_other.raw_ptr_; }

    T* operator->() noexcept { return get(); }
    T operator*() { return *get(); }

    explicit operator bool() { return raw_ptr_ != nullptr; }
    
  private:
    T* raw_ptr_ {nullptr};
};//class

template<typename T>
  requires (!std::is_array_v<T>)
sp::owning_ptr<T> make_owning() 
{
  return sp::owning_ptr<T>(new T());
}

template<typename T>
  requires std::is_bounded_array_v<T>
sp::owning_ptr<T> make_owning(std::size_t p_size) 
{
  return sp::owning_ptr<T>(new std::remove_extent_t<T>[p_size]);
}

template<typename T, typename U>
  requires (!std::is_array_v<T>)
sp::owning_ptr<T> make_owning(U&& p_u) 
{
  return sp::owning_ptr<T>(new T(std::forward<U>(p_u)));
}

///FIXME::ASAN Leak report
template<typename T, typename... Args>
   requires (!std::is_array_v<T>)
sp::owning_ptr<T> make_owning(Args&&... p_args) 
{
  return sp::owning_ptr<T>( new T(std::forward<Args>(p_args)...) );  
}

template<typename T, typename... Args>
  requires std::is_bounded_array_v<T>
sp::owning_ptr<T> make_owning(Args&&... p_args) = delete;

}//sp namespace


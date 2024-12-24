#pragma once

namespace sp
{

template<typename T>
class Option
{
  public:
    Option() = default;
    Option(T& p_value) : value_(&p_value) {}
    ~Option()
    {
      if(HasValue()) {
        if constexpr(!std::is_trivially_constructible<T>::value) {
          value_->~T();
        }
        value_ = nullptr;
      }
    }

    bool HasValue() const { return value_ != nullptr; }
  
    const T& Value() const { return *value_; }
    T& Value() { return *value_; }
  
    const T& ValueOr(const T& p_other) const 
    {
      if(HasValue()) {
        return Value();
      }
      return p_other;
    }

    T& ValueOr(T& p_other)
    {
      if(HasValue()) {
        return Value();
      }
      return p_other;
    }

    const T* operator->() const { return value_; }
    T* operator->() { return value_; }
    const T& operator*() const { return Value(); }
    T& operator*() { return Value(); }

    explicit operator bool() const { return value_ != nullptr; }
    bool operator !=(const Option& p_rhs) const { return *value_ != *p_rhs.value_; }
    bool operator ==(const Option& p_rhs) const { return *value_ == *p_rhs.value_; }

  private:
    T* value_ {nullptr};
};

} // namespace sp

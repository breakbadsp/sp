#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <utility>

namespace sp 
{
  template<typename T>
  class vector 
  {
  public:
    constexpr vector() = default;
    ~vector() 
    { 
      clear(); 
    }

    vector(size_t p_capacity) 
      : buffer_(alloc(p_capacity))
      , capacity_(p_capacity)
      , size_(p_capacity) 
    {
      for (size_t i = 0; i < size_; ++i) 
      {
        std::construct_at(slot(i));
      }
    }

    vector(const vector &p_rhs) 
      : buffer_(alloc(p_rhs.capacity_))
      , capacity_(p_rhs.capacity_)
      , size_(p_rhs.size_) 
    {
      for (size_t i = 0; i < size_; ++i) 
      {
        std::construct_at(slot(i), *p_rhs.slot(i));
      }
    }

    vector(vector &&p_rhs) noexcept 
      : buffer_(p_rhs.buffer_)
      , capacity_(p_rhs.capacity_)
      , size_(p_rhs.size_) 
    {
      p_rhs.buffer_ = nullptr;
      p_rhs.capacity_ = 0;
      p_rhs.size_ = 0;
    }

    vector &operator=(vector p_rhs) noexcept
    {
      swap(p_rhs);
      return *this;
    }

    friend void swap(vector &p_lhs, vector &p_rhs) noexcept 
    {
      using std::swap;
      swap(p_lhs.buffer_, p_rhs.buffer_);
      swap(p_lhs.capacity_, p_rhs.capacity_);
      swap(p_lhs.size_, p_rhs.size_);
    }

    void swap(vector &p_other) noexcept 
    {
      using std::swap;
      swap(buffer_, p_other.buffer_);
      swap(capacity_, p_other.capacity_);
      swap(size_, p_other.size_);
    }

    const T &operator[](size_t p_idx) const { return *slot(p_idx); }
    T &operator[](size_t p_idx) { return *slot(p_idx); }

    void push_back(const T &p_rhs) 
    {
      if (size_ == capacity_) 
      {
        reallocate(capacity_ * 2);
      }
      std::construct_at(slot(size_), p_rhs);
      ++size_;
    }

    void push_back(T &&p_rhs) 
    {
      if (size_ >= capacity_) 
      {
        reallocate(capacity_ * 2);
      }
      std::construct_at(slot(size_), std::move(p_rhs));
      ++size_;
    }

    T pop_back() 
    {
      assert(size_ > 0);
      auto ele = std::move(*last());
      std::destroy_at(last());
      --size_;
      return ele;
    }


    void reserve(size_t p_cap) {
      buffer_ = alloc(p_cap);
      capacity_ = p_cap;
    }

    void resize(size_t p_nsize) 
    {
      if (size_ == p_nsize)
      {
        return;
      }

      if (size_ > p_nsize) 
      {
        size_ = p_nsize;
        return;
      }

      reallocate(p_nsize);
      for (size_t i = size_; i < p_nsize; ++i) 
      {
        std::construct_at(slot(i));
      }
      size_ = p_nsize;
    }

    void resize(size_t p_nsize, const T &p_default) 
    {
      if (size_ == p_nsize)
      {
        return;
      }

      if (size_ > p_nsize) 
      {
        size_ = p_nsize;
        return;
      }

      reallocate(p_nsize);
      for (size_t i = size_; i < p_nsize; ++i) 
      {
        std::construct_at(slot(i), p_default);
      }
      size_ = p_nsize;
    }


    const T &back() const { return *last(); }
    T &back() { return *last(); }

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }

    T *data() { return reinterpret_cast<T *>(buffer_); }
    const T *data() const { return reinterpret_cast<T *>(buffer_); }

    bool empty() const noexcept { return size() == 0; }

  private:
    void reallocate(size_t p_ncap) 
    {
      p_ncap = p_ncap == 0 ? 2 : p_ncap;
      T *new_buff = alloc(p_ncap);
      for (size_t i = 0; i < size_; ++i) 
      {
        if constexpr (std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) 
        {
          std::construct_at(new_buff + i, std::move(*slot(i)));
        } 
        else 
        {
          std::construct_at(new_buff + i, *slot(i));
        }
      }

      clear();
      capacity_ = p_ncap;
      buffer_ = reinterpret_cast<T *>(new_buff);
    }

    T *slot(size_t idx) { return reinterpret_cast<T *>(buffer_ + idx); }

    const T *slot(size_t idx) const { return reinterpret_cast<T *>(buffer_ + idx); }

    const T *last() const { return slot(size_ - 1); }
    T *last() { return slot(size_ - 1); }


    void clear() 
    {
      for (size_t i = 0; i < size_; ++i) 
      {
        std::destroy_at(slot(i));
      }
      free(buffer_);
      buffer_ = nullptr;
    }

    T *alloc(size_t p_size) 
    {
      if(p_size == 0) return nullptr;
      return reinterpret_cast<T *>(std::malloc(sizeof(T) * p_size));
    }

  private:
    T *buffer_ = nullptr;
    size_t capacity_{0};
    size_t size_{0};
  };
} // namespace sp

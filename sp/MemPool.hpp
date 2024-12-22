#pragma once

#include <vector> //TODO:: replace with sp::vector
#include <atomic>
#include <cassert>
#include <iostream>

namespace sp
{
//TODO:: make it thread safe
template <typename T>
class MemPool final
{
  public:
    MemPool(const size_t p_size)
    : store_(p_size, {T(), true})
    , next_free_index_(0)
    {
      //TODO:: assert object is stored in block first => for deallocation
    }

    MemPool() = delete;
    MemPool(const MemPool&) = delete;
    MemPool(MemPool&&) = delete;
    MemPool& operator =(const MemPool&) = delete;
    MemPool& operator =(MemPool&&) = delete;

    template<typename... Arg>
    T* Allocate(Arg... p_arg) noexcept
    {
      auto& block = (store_[next_free_index_]);
      assert(block.is_free_ == true);
      T* ret = &(block.object_);
      ret = new(ret) T(std::forward<Arg>(p_arg)...); //placement new
      block.is_free_ = false;
      UpdateNextFreeIndex();
      return ret;
    }

    auto Deallocate(const T* p_ele) noexcept
    {
      const auto index = (reinterpret_cast<const ObjectBlock*>(p_ele) - &(store_[0]));
      assert(index >= 0);
      auto& block = store_[index];
      assert(block.is_free_ == false);
      if constexpr(!std::is_trivially_constructible<T>::value)
        block.object_.~T();
      block.is_free_ = true;
    }

    size_t size() const { return store_.size(); }

  private:
    struct ObjectBlock
    {
      T object_;
      bool is_free_;
    };

    std::vector<ObjectBlock> store_ {0};
    std::atomic<size_t> next_free_index_ {0};

  private:
    auto UpdateNextFreeIndex()
    {
      const u_int32_t old_index = next_free_index_.load();
      u_int32_t new_index = next_free_index_.load();
      while (!store_[new_index].is_free_)
      {
        new_index = ((new_index + 1) % store_.size());
        assert(old_index != new_index);
      }
      next_free_index_.store(new_index);
    }
};
} // namespace sp
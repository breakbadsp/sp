#include <vector> //TODO:: replace with sp::vector
#include <atomic>

//TODO:: make it thread safe
template <typename T>
class MemPool final
{
  public:
    MemPool(const size_t p_size)
    : store_(p_size, {T(), true})
    : next_free_index_(0)
    {
      //TODO:: assert object is stored in block first => for deallocation
    }

    MemPool() = delete;
    MemPool(const MemPool&) = delete;
    MemPool(MemPool&&) = delete;
    MemPool& operator =(const MemPool&) = delete;
    MemPool& operator =(const MemPool&) = delete;

    template<typename... Arg>
    T* Allocate(Arg... p_arg) noexcept
    {
      //static_assert store is not full
      auto block = &(store_[next_free_index_]);
      //static_assert block is free
      T* ret = &(block.object_);
      ret = new(ret) (std::forward<Arg>(p_erg)...); //placement new
      block.is_free_ = false;
      UpdateNextFreeIndex();
      return ret;
    }

    auto Deallocate(const T* p_ele) noexcept
    {
      const auto index = (reinterpret_cast<ObjectBlock*>(p_ele) - &(store_[0]));
      //TODO:: assert !is_free
      //TODO:: assert index >=0 
      store_[index].is_free_ = true;
    }

  private:
    struct ObjectBlock
    {
      T object_;
      bool is_free_;
    };

    std::vector<ObjectBlock> store_;
    std::atomic<size_t> next_free_index_ {0};
    //TODO:: optimize with freed_indexes

  private:
    auto UpdateNextFreeIndex()
    {
      const auto old_index = next_free_index_;
      while (!store_[next_free_index_].is_free_)
      {
        next_free_index_ = (next_free_index_ + 1) % store_.size();
        if( [[unlikely]] old_index == next_free_index_) 
          break;//store is full
      }
    }
};

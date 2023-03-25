#include <atomic>
#include <shared_mutex>
#include <mutex>

namespace sp{

template<typename T>
class shared_ptr_control_block
{
  public:
    shared_ptr_control_block() = default;

    bool IncreaseRefCount() {
      std::unique_lock ul(ref_count_mutex_);
      if(ref_count_.load() <= 0)
        return false;

      ++ref_count_;
      return true;
    }

    bool DecreaseRefCount(shared_ptr<T>& p_shared_ptr) {
      std::unique_lock ul(ref_count_mutex_);
      if(ref_count_.load() <= 0)
        return false;

      --ref_count_;
      if(ref_count_ == 0)
        p_shared_ptr.Clear();
    }

  private:
    std::atomic_uint ref_count_ {1};
    std::shared_mutex ref_count_mutex_;
};

template<typename T>
class shared_ptr
{
  shared_ptr()
  {
    raw_ptr_ = nullptr;
    control_block_ = {};
  }

  ~shared_ptr() {
    control_block_.DecreaseRefCount(*this);
  }

  //move
  shared_ptr(const shared_ptr& p_other) {
    raw_ptr_ = p_other.raw_ptr_;
    control_block_ = p_other.control_block;
    control_block_.IncreaseRefCount();
  }

  shared_ptr& operator=(const shared_ptr& p_other) {
    if(this == &p_other)
      return;
    raw_ptr_ = p_other.raw_ptr_;
    control_block_ = p_other.control_block;
    control_block_.IncreaseRefCount();
    return *this;
  }

  Clear()
  {
    if(std::is_unbounded_array_v<T> || std::is_bounded_array<T>)
      delete [] raw_ptr_;
    else
      delete raw_ptr_;
    raw_ptr_ = nullptr;
  }

  private:
    T* raw_ptr_  {nullptr};
    shared_ptr_control_block& control_block_;
};
} //sp
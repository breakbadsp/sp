#include <atomic>
#include <shared_mutex>
#include <mutex>

namespace sp{

template<typename T>
class shared_ptr {
  public:
    class shared_ptr_control_block
    {
      public:
        shared_ptr_control_block() = default;

        bool IncreaseRefCount() {
          std::unique_lock ul(ref_count_mutex_);
          //std::cout << "ref count before increase is " << ref_count_.load() << '\n';
          if(ref_count_.load() <= 0)
            return false;

          ++ref_count_;
          //std::cout << "ref count after increase is " << ref_count_.load() << '\n';
          return true;
        }

        bool DecreaseRefCount() {
          std::unique_lock ul(ref_count_mutex_);
          //std::cout << "ref count before decrease is " << ref_count_.load() << '\n';
          if(ref_count_.load() <= 0)
            return false;

          --ref_count_;
          //std::cout << "ref count after decrease is " << ref_count_.load() << '\n';
          if(ref_count_.load() == 0)
          {
            return false;
          }
          return true;
        }

      private:
        std::atomic_uint ref_count_ {1};
        std::shared_mutex ref_count_mutex_;
    };
  public:
    shared_ptr()
    : raw_ptr_(nullptr)
    , control_block_(nullptr) {
    }

    shared_ptr(T* p_raw_ptr) 
    : raw_ptr_(p_raw_ptr)
    , control_block_(new shared_ptr_control_block()) {

    }

    shared_ptr& operator=(decltype(nullptr)){
      if(control_block_ and !control_block_->DecreaseRefCount()) {
        Clear();
      }
      raw_ptr_ = nullptr;
      control_block_ = nullptr;
      return *this;
    }

    ~shared_ptr() {
      if(control_block_ and !control_block_->DecreaseRefCount())
      {
        Clear();
      }
    }

    //copy
    shared_ptr(const shared_ptr& p_other) 
    : raw_ptr_(p_other.raw_ptr_)
    , control_block_(p_other.control_block_)
    {
      if(control_block_)
        control_block_->IncreaseRefCount();
    }

    shared_ptr(shared_ptr&& p_other) 
    : raw_ptr_(std::move(p_other.raw_ptr_))
    , control_block_(std::move(p_other.control_block_))
    {}

    shared_ptr& operator=(const shared_ptr& p_other) {
      if(this == &p_other)
        return *this;

      if(raw_ptr_) {
        if(control_block_ and !control_block_->DecreaseRefCount())
          Clear();
      }
      
      //FIXME: solve memory leak of old resources
      raw_ptr_ = p_other.raw_ptr_;
      control_block_ = p_other.control_block_;

      if(control_block_)
        control_block_->IncreaseRefCount();
        
      return *this;
    }

    shared_ptr& operator=(shared_ptr&& p_other) {
      if(this == &p_other)
        return *this;

      raw_ptr_ = std::move(p_other.raw_ptr_);
      control_block_ = std::move(p_other.control_block_);
  
      return *this;
    }
    
    shared_ptr& operator=(T* p_raw_ptr) {
      if(raw_ptr_ == p_raw_ptr)
        return *this;

      if(raw_ptr_)
      {
        if(control_block_ and !control_block_->DecreaseRefCount())
          Clear();
      }
      
      raw_ptr_ = p_raw_ptr;
      control_block_ = new shared_ptr_control_block();       
      return *this;
    }

    const T* get() const { return raw_ptr_; }
    T* get() { return raw_ptr_; }

    //with raw
    bool operator==(T* p_raw) { return p_raw == raw_ptr_; }
    bool operator!=(T* p_raw) { return p_raw != raw_ptr_; }

    bool operator==(std::nullptr_t p_null) { return p_null == raw_ptr_; }
    bool operator!=(std::nullptr_t p_null) { return p_null != raw_ptr_; }

    bool operator==(const shared_ptr& p_other) { return raw_ptr_ == p_other.raw_ptr_; }
    bool operator!=(const shared_ptr& p_other) { return raw_ptr_ != p_other.raw_ptr_; }

    T* operator->() { return get(); }
    T operator*() { return *get(); }

    explicit operator bool() { return raw_ptr_ != nullptr; }

    //Move

  private:
    T* raw_ptr_  {nullptr};
    shared_ptr_control_block* control_block_;

    void Clear()
    {
      //std::cout << "clearing shread_ptr\n";
      if(std::is_unbounded_array_v<T>)// || std::is_bounded_array<T>)
        delete [] raw_ptr_;
      else
        delete raw_ptr_;
      raw_ptr_ = nullptr;
      
      delete control_block_;
      control_block_ = nullptr;
    }
};
} //sp
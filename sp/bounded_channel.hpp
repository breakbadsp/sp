#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

namespace sp {
template<typename T>
class bounded_channel
{
public:
    bounded_channel() = delete;
    bounded_channel(const bounded_channel&) = delete;
    bounded_channel(bounded_channel&&) = delete;
    bounded_channel& operator=(const bounded_channel&) = delete;
    bounded_channel& operator=(bounded_channel&&) = delete;

    bounded_channel(size_t p_cap)
    : capacity_(p_cap)
    , size_(0)
    , ri_(0)
    , wi_(0)
    {
      buffer_.reserve(p_cap);
    }

    void enqueue(const T& p_val)
    {
        std::unique_lock ul(mut_);
        while (full()) {
          cv_.wait(ul);
        }
        buffer_[wi_] = p_val;
        wi_ = (wi_ + 1) % capacity_;
        size_++;
        assert(size_ <= capacity_);
        cv_.notify_one();
    }

    T dequeue()
    {
        std::unique_lock ul(mut_);
        while (empty()) {
          cv_.wait(ul);
        }
        const auto val = std::move(buffer_[ri_]);
        ri_ = (ri_ + 1) % capacity_;
        size_--;
        assert(size_ <= capacity_);
        cv_.notify_one();
        return val;
    }

    size_t size() const { return size_; }
    size_t capacity() const  { return capacity_; }
    bool full() const { return size_ == capacity_; }
    bool empty() const { return size_ == 0; }

  private:
    std::vector<T> buffer_;
    size_t capacity_{0};
    size_t size_ {0};
    std::size_t ri_ {0};
    std::size_t wi_ {0};

    std::mutex mut_;
    std::condition_variable cv_;
};
}//sp



/// [                                      <- ]
///               .  .   .   4   3   2   1   0   
///                                <= enque increase the write index wi = (wi + 1) % capacity;
/// [                              <= dequeue increases the read index
/// empty read index = -1 or 
/// full if size_ == capacity


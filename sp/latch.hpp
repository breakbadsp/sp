#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace sp {
class latch {
public:
  latch() = delete;
  latch(const latch&) = delete;
  // Test this: If CopyConstructor is deleted,
  // compiler should not generate any other special ops

  latch(int p_count ) noexcept
  : count_(p_count)
  {}

  void count_down() noexcept {
    --count_;
    if(count_.load() == 0)
    {
      //TODO:: Test calling notify all without holding lock.
      cv_.notify_all();
    }
  }

  void wait() const noexcept {
    std::unique_lock<std::mutex> ul(mut_);
    while(count_.load() != 0) {
      cv_.wait(ul);
    }
  }

  bool try_wait() const noexcept {
    return count_.load() == 0;
  }

  void arrive_and_wait() noexcept {
    count_down();
    std::unique_lock<std::mutex> ul(mut_);
    while(count_.load() != 0) {
      cv_.wait(ul);
    }
    cv_.notify_all();
  }


private:
  std::atomic_size_t count_ = {0};
  mutable std::condition_variable cv_;
  mutable std::mutex mut_;

  //TODO:: explore some lockless mechanisms
};
}//sp
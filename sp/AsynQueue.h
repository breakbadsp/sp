#pragma once

#include <vector>
#include <mutex>
#include <utility>
#include <atomic>
#include <condition_variable>

namespace sp {
template<typename T>
class AsyncQueue
{
  public:
    AsyncQueue(size_t p_size, size_t p_count) : 
     size_(p_size)
    ,dequeu_count_(p_count)
    {
      queue_.reserve(size_);
    }

    AsyncQueue() = default;
    AsyncQueue(const AsyncQueue&) = delete;
    AsyncQueue& operator=(const AsyncQueue&) = delete;
    AsyncQueue(AsyncQueue&&) = delete;
    AsyncQueue& operator=(AsyncQueue&&) = delete;

    void Enqueue(T&& p_data)
    {
      {
        std::unique_lock<std::mutex> ulock(queue_lock_);
        queue_.emplace_back(std::forward<T>(p_data));
      }
      cv_.notify_one();
    }

    T Dequeue()
    {
      std::unique_lock<std::mutex> ulock(queue_lock_);
      while(queue_.empty)
        cv_.wait(ulock);

      auto ele = queue_.front();
      queue_.pop_front();
      return ele;
    }

    void EnqueueBulk(T&& p_data)
    {
      std::unique_lock<std::mutex> ulock(queue_lock_);
      queue_.emplace_back(std::forward<T>(p_data));
      if(queue_.size() >= dequeu_count_)
      {
        cv_.notify_one();
      }
    }

    void BlockingEnqueueBulk(T&& p_data)
    {
      std::unique_lock<std::mutex> ulock(queue_lock_);
      queue_.emplace_back(std::forward<T>(p_data));
      if(queue_.size() >= dequeu_count_)
      {
        cv_.notify_one();
        while (!queue_.empty())
        {
          cv_.wait(ulock);
        }
        
      }
    }

    std::vector<T> DequeueBulk()
    {
      std::vector<T> data_copy;
      {
        std::unique_lock<std::mutex> ulock(queue_lock_);
        while(queue_.empty() || queue_.size() < dequeu_count_)
        {
          cv_.wait(ulock);
        }
        data_copy = queue_;
        queue_.clear();
      }
      cv_.notify_one();
      return data_copy;
    }

  private:
    std::vector<T> queue_;
    size_t size_  {10};
    size_t dequeu_count_ {1};
    std::mutex queue_lock_;
    std::condition_variable cv_;
    std::atomic<bool> ready_  {true};
};
}//sp
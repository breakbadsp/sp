#pragma once

#include <vector>
#include <mutex>
#include <utility>
#include <atomic>
#include <condition_variable>

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
        cv_.notify_one();
      }
    }

    void EnqueueBulk(T&& p_data)
    {
      std::unique_lock<std::mutex> ulock(queue_lock_);
      std::cout << "Enqueued " << p_data << '\n';
      queue_.emplace_back(std::forward<T>(p_data));
      if(queue_.size() >= dequeu_count_)
      {
        cv_.notify_one();
      }
    }

    std::vector<T> Dequeue(int p_len)
    {
      std::unique_lock<std::mutex> ulock(queue_lock_);
      while(queue_.size() < p_len)
      {
        cv_.wait(ulock);
      }
      const auto data_copy = queue_;
      //Remove this
      std::cout << "Dequeued data with size =" << data_copy.size() << '\n';
      for (const auto& ele : data_copy)
        std::cout << ele << ", ";
      //
      queue_.clear();
      return data_copy;
    }

  private:
    std::vector<T> queue_;
    size_t size_  {10};
    size_t dequeu_count_ {};
    std::mutex queue_lock_;
    std::condition_variable cv_;
    std::atomic<bool> ready_  {true};
};
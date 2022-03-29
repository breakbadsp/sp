#include <vector>
#include <mutex>
#include <condition_variable>

template<typename T>
class AsyncQueue
{
  public:
    void Enqueue(T p_data)
    {
      {
        std::unique_lock<std::mutex> lock(queue_lock_);
        queue_.emplace_back(p_data);
      }
      cv_.notify_one();
    }

    std::vector<T> Dequeue(int p_len)
    {
      std::unique_lock<std::mutex> ulock(queue_lock_);
      while(queue_.size() < p_len)
      {
        cv_.wait(ulock);
      }
      auto data_copy = queue_;
      queue_.clear();
      return data_copy;
    }

  private:
    std::vector<T> queue_;
    std::mutex queue_lock_;
    std::condition_variable cv_;
};
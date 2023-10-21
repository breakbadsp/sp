// Author: Sachin Pandhare
// A very simple MultiProducerSingleConsumer lockless queue
// It is an attempt at solving the problem.


#pragma once

#include <vector> //TODO:: use sp::vector
#include <atomic>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <shared_mutex>
#include <string>

namespace sp
{

// TODO:: Make sure this is tested :)
template<typename T>
class mpscllqueue
{
  public:
    constexpr static size_t MAX_QUEUE_SIZE = 10000;
    constexpr static size_t UNUSED_QUEUE_BUFFER = 1000;

    void enqueue(T&& p_ele)
    {
      //Full
      auto new_read_index = (read_index_.load() - 1) % MAX_QUEUE_SIZE; 
      while(write_index_.compare_exchange_weak(new_read_index, write_index_.load(), std::memory_order::seq_cst))
      {
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
        new_read_index = (read_index_.load() - 1) % MAX_QUEUE_SIZE;
      }
    
      //write
      auto new_write_index = write_index_.fetch_add(1);
      new_write_index = (new_write_index % MAX_QUEUE_SIZE);

      new_read_index = (read_index_.load() - 1) % MAX_QUEUE_SIZE; 
      while(write_index_.compare_exchange_weak(new_read_index, new_write_index, std::memory_order::seq_cst))
      {
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
        new_read_index = (read_index_.load() - 1) % MAX_QUEUE_SIZE;
      }

      store_[new_write_index] = std::forward<T>(p_ele);
      cv_.notify_all();
    }

    T dequeue()
    {
      // It is a single consumer queue, 
      // There will not be two reader calling this function simultaneously.
      
      //EmptySync
      auto write_index = write_index_.load(std::memory_order::seq_cst);
      while(read_index_.compare_exchange_strong(write_index, read_index_.load(), std::memory_order::seq_cst))
      {
        //do synchronization only if queue is empty
        std::unique_lock ul(mutex_);
        cv_.wait(ul);
        write_index = write_index_.load(std::memory_order::seq_cst);
      }
      
      //read
      auto new_read_index = read_index_.fetch_add(1);
      new_read_index = (new_read_index % MAX_QUEUE_SIZE);
      return store_[new_read_index];
    }

  private:
    std::vector<T> store_ {MAX_QUEUE_SIZE + UNUSED_QUEUE_BUFFER};
    std::atomic<size_t> read_index_ {0};
    std::atomic<size_t> write_index_ {0};
    std::shared_mutex mutex_;
    std::condition_variable cv_;
};
}//namespace sp

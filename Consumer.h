#pragma once

#include <iostream>
#include <numeric>

#include "AsynQueue.h"

template<typename T>
class Consumer
{
  public:
    Consumer(AsyncQueue<T>& p_queue) : queue_(p_queue) {}

    void Run()
    {
      while(1)
      {
        const auto data = queue_.Dequeue(5);
        std::cout << "Recieved data with size=" << data.size() << std::endl;
        for (const auto ele : data)
          std::cout << ele << ", ";
        std::cout << "Total=" << std::accumulate(data.begin(), data.end(), 0) << std::endl;
      }
    }
  private:
    AsyncQueue<T>& queue_;
};


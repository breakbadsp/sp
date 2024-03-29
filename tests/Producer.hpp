#pragma once

#include <thread>
#include <iostream>

#include "AsyncQueue.h"

namespace sp {
template<typename T>
class Producer
{
  public:
    Producer(AsyncQueue<T>& p_queue) : queue_(p_queue) {}

    void Run()
    {
      while(1)
      {
        T i = 0;
        using namespace std::chrono_literals;
        while(i < 100)
        {
          //queue_.EnqueueBulk(i++);
          queue_.BlockingEnqueueBulk(i++);
          //std::this_thread::sleep_for(100ms);
        }
        //std::cout << "Done sending data." << std::endl;
      }
    }
  private:
    AsyncQueue<T>& queue_;
};
} //sp
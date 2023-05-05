#pragma once

#include <iostream>
#include <numeric>

#include "AsyncQueue.h"

namespace sp{

template<typename T>
class Consumer
{
  public:
    enum class Action
    {
         FindTotal
        ,FindAverage
    };

    Consumer(AsyncQueue<T>& p_queue) : 
     queue_(p_queue)
     {}

    void Run()
    {
      while(1)
      {
        auto data = queue_.DequeueBulk();
        Process(data, Action::FindTotal);
      }
    }

    void Process(std::vector<T>& p_data, Action p_action)
    {
        switch(p_action)
        {
            case Action::FindTotal:
                std::cout << "Total=" << std::accumulate(p_data.begin(), p_data.end(), 0) << std::endl;
                break;
            case Action::FindAverage:
                std::cout << "Avg=" << std::accumulate(p_data.begin(), p_data.end(), 0) / p_data.size() << std::endl;
                break;
            default: break;
        }
    }

  private:
    AsyncQueue<T>& queue_;
};
} //sp
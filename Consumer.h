#pragma once

#include <iostream>
#include <numeric>

#include "AsynQueue.h"

template<typename T>
class Consumer
{
  public:
    enum class Action
    {
         FindTotal
        ,FindAverage
    };

    Consumer(AsyncQueue<T>& p_queue, size_t p_count) : 
     queue_(p_queue)
    ,consume_ele_count_(p_count > 0 ? p_count : 1)
    {}

    void Run()
    {
      while(1)
      {
        auto data = queue_.Dequeue(consume_ele_count_);
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
    size_t consume_ele_count_;
};


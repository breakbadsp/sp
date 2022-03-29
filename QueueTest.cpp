#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <numeric>

#include "lib/AsynQueue.h"

template<typename T>
class Producer
{
  public:
    Producer(AsyncQueue<T>& p_queue) : queue_(p_queue) {}

    void Run()
    {
      while(1)
      {
        int i = 0;
        using namespace std::chrono_literals;
        while(i < 100)
        {
          queue_.Enqueue(i++);
          //std::this_thread::sleep_for(10ms);
        }
        std::cout << "Done sending data" << std::endl;
      }
    }
  private:
    AsyncQueue<T>& queue_;
};

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


int main()
{
  AsyncQueue<int> q;
  Producer<int> p(q);
  Consumer<int> c(q);
  std::thread producer_thread(&Producer<int>::Run, &p);
  std::thread consumer_thread(&Consumer<int>::Run, &c);

  producer_thread.join();
  producer_thread.join();
  return 0;
}
#include <iostream>
#include <thread>
#include <chrono>

#include "lib/AsynQueue.h"


class Producer
{
  public:
    Producer(AsyncQueue<int>& p_queue) : queue_(p_queue) {}

    void Run()
    {
      while(1)
      {
        int i = 0;
        using namespace std::chrono_literals;
        while(i < 100)
        {
          queue_.Enqueue(i++);
          std::this_thread::sleep_for(500ms);
        }
        std::cout << "Done sending data" << std::endl;
      }
    }
  private:
    AsyncQueue<int>& queue_;
};

class Consumer
{
  public:
    Consumer(AsyncQueue<int>& p_queue) : queue_(p_queue) {}

    void Run()
    {
      while(1)
      {
        const auto data = queue_.Dequeue(5);
        std::cout << "Recieved data with size=" << data.size() << std::endl;
        for (const auto ele : data)
          std::cout << ele << ", ";
        std::cout << std::endl;
      }
    }
  private:
    AsyncQueue<int>& queue_;
};


int main()
{
  AsyncQueue<int> q;
  Producer p(q);
  Consumer c(q);
  std::thread producer_thread(&Producer::Run, &p);
  std::thread consumer_thread(&Consumer::Run, &c);

  producer_thread.join();
  producer_thread.join();
  return 0;
}
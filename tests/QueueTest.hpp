#include "Producer.hpp"
#include "Consumer.hpp"

void TestQueue(int count)
{
  sp::AsyncQueue<int> q(count+10, count);
  sp::Producer<int> p(q);
  sp::Consumer<int> c(q);
  std::thread consumer_thread(&sp::Consumer<int>::Run, &c);
  std::thread producer_thread(&sp::Producer<int>::Run, &p);

  producer_thread.join();
  consumer_thread.join();
  return 0;
}

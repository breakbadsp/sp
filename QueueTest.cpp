#include "Producer.h"
#include "Consumer.h"

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

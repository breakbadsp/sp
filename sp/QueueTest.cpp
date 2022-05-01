#include "Producer.h"
#include "Consumer.h"

int main(int argc, char** argv)
{
  if(argc <= 1)
  {
    std::cout << "Please enter consumer bulk read count " << std::endl;
    return 0;
  }
  const auto count = atoi(argv[1]);
  sp::AsyncQueue<int> q(count+10, count);
  sp::Producer<int> p(q);
  sp::Consumer<int> c(q);
  std::thread consumer_thread(&sp::Consumer<int>::Run, &c);
  std::thread producer_thread(&sp::Producer<int>::Run, &p);

  producer_thread.join();
  consumer_thread.join();
  return 0;
}

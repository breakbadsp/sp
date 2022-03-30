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
  AsyncQueue<int> q(count+10, count);
  Producer<int> p(q);
  Consumer<int> c(q, atoi(argv[1]));
  std::thread producer_thread(&Producer<int>::Run, &p);
  std::thread consumer_thread(&Consumer<int>::Run, &c);

  producer_thread.join();
  producer_thread.join();
  return 0;
}

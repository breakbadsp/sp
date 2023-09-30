
#include "VectorTests.hpp"
#include "StringTest.hpp"
#include "ListTest.hpp"
#include "QueueTest.hpp"
#include "TestSharedPtr.hpp"
#include "MapTest.hpp"
#include "CMN.hpp"
#include "cmn.hpp"
#include "OrderBook.hpp"

int main()
{
  std::vector<std::thread*> threads;
  TestCmn();


  std::cout << "Testing order book\n";

  int order_id = 0;

  OrderBook::Process({order_id++, "REL", 99.01, 100, OrderEvent::Type::New, Side::Buy});
  OrderBook::Process({order_id++, "REL", 99.01, 100, OrderEvent::Type::New, Side::Sell});

  OrderBook::Process({order_id++, "REL", 99.01, 100, OrderEvent::Type::New, Side::Buy});
  OrderBook::Process({order_id++, "REL", 99.01, 200, OrderEvent::Type::New, Side::Buy});
  OrderBook::Process({order_id++, "REL", 99.01, 300, OrderEvent::Type::New, Side::Buy});
  OrderBook::Process({order_id++, "REL", 99.01, 300, OrderEvent::Type::New, Side::Sell});

  OrderBook::Process({order_id++, "REL", 99.02, 100, OrderEvent::Type::New, Side::Buy} );
  OrderBook::Process({order_id++, "REL", 99.03, 100, OrderEvent::Type::New, Side::Buy} );
  OrderBook::Process({order_id++, "REL", 99.04, 100, OrderEvent::Type::New, Side::Buy} );
  OrderBook::Process({order_id++, "REL", 99.05, 100, OrderEvent::Type::New, Side::Buy} );
  
  OrderBook::Process({order_id++, "REL", 99.06, 100, OrderEvent::Type::New, Side::Sell} );
  OrderBook::Process({order_id++, "REL", 99.07, 100, OrderEvent::Type::New, Side::Sell} );
  OrderBook::Process({order_id++, "REL", 99.08, 100, OrderEvent::Type::New, Side::Sell} );
  OrderBook::Process({order_id++, "REL", 99.09, 100, OrderEvent::Type::New, Side::Sell} );

  OrderBook::Process({order_id++, "REL", 99.05, 100, OrderEvent::Type::New, Side::Sell} );
  OrderBook::Process({order_id++, "REL", 99.04, 100, OrderEvent::Type::New, Side::Sell} );
  OrderBook::Process({order_id++, "REL", 99.06, 100, OrderEvent::Type::New, Side::Buy} );

  
  using namespace sp;
  //if(argc <= 1)
    //threads.emplace_back(CreateAndRunThread(-1, "TestQueue", TestQueue, 1));
  //else
    //threads.emplace_back(CreateAndRunThread(0, "TestQueue", TestQueue, atoi(argv[1])));
  /*
  threads.emplace_back(CreateAndRunThread(1, "TestVectorPushBack", TestVectorPushBack));
  threads.emplace_back(CreateAndRunThread(2, "TestListApis", TestListApis));
  threads.emplace_back(CreateAndRunThread(3, "TestString", TestString));
  threads.emplace_back(CreateAndRunThread(4, "TestSharedPtr", TestSharedPtr));
  threads.emplace_back(CreateAndRunThread(5, "TestMap", TestMap));

  for(auto* t: threads)
  {
    t->join();
    delete t;
  }
  */
  std::cout << "test are done\n";
}

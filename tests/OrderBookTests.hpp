#pragma once

#include "OrderBook.hpp"

void TestOrderBook()
{
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
}
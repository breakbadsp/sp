#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

  enum class Side
  {
    Buy,
    Sell
  };

struct OrderEvent
{
  enum class Type
  {
    New,
    Rpl,
    Cxl
  };

  Type type_ {Type::New};
  std::string order_id_;
  std::string symbol_;
  int size_ {0};
  double price_ {0};
  Side side_ {Side::Buy};

};

struct Order
{
  Order(const OrderEvent& p_event)
  {
    FromEvent(p_event);
  }

  void FromEvent(const OrderEvent& p_event)
  {
    price_ = p_event.price_;
    size_ = p_event.size_;
    time_ = time(0);
    id_ = p_event.order_id_;
    side_ = p_event.side_;
  }

  double price_{0};
  int size_{0};
  int time_{0};
  std::string id_;
  Side side_ {Side::Buy};

  bool operator<(const Order &p_rhs)
  {
    return time_ < p_rhs.time_;
  }
};

class Level
{
public:
  int size_{0};
  double price_{0};
  std::multiset<Order> orders_;

  bool AddOrder(Order&& p_order)
  {
    std::cout << "Order id " << p_order.id_ << " Added\n";
    size_ += p_order.size_;
    orders_.insert(std::move(p_order));
    return true;
  }

  bool CancelOrder(const Order& p_order) //FIXME:: optimize
  {
    for(auto itr = orders_.begin(); itr != orders_.end(); ++itr)
    {
      if(itr->id_ == p_order.id_)
      {
        std::cout << "Order id " << p_order.id_ << " Cancelled\n";
        size_ -= itr->size_;
        itr = orders_.erase(itr);
        break;
      }
    }
    return true;
  }

  bool Match(const Order& p_order)
  {
    std::cout << "Order id " << p_order.id_ << " matched\n";
    int remaining_qty = p_order.size_;
    for(auto itr = orders_.begin(); itr != orders_.end(); ++itr)
    {
      size_ -= itr->size_;
      if(itr->size_ < remaining_qty)
      {
        remaining_qty -= itr->size_;
        itr = orders_.erase(itr);
        continue;
      }
      else if(itr->size_ > remaining_qty)
      {
        itr->size_ -= remaining_qty;
        break;
      }

      itr = orders_.erase(itr);
      break;
      //equal
    }
    return true;
  }
};

class BuyLevel : public Level
{
public:
  const Side side_{Side::Buy};

  bool operator<(const BuyLevel& p_rhs)
  {
    return price_ > p_rhs.price_;
  }
};

class SellLevel : public Level
{
public:
  const Side side_{Side::Sell};

  bool operator<(const BuyLevel& p_rhs)
  {
    return price_ < p_rhs.price_;
  }
};

class OrderBook
{
  static bool Process(const OrderEvent& p_event)
  {
    Order order(p_event);
    switch (p_event.type_)
    {
    case OrderEvent::Type::New:
      {
        auto itr = s_order_book_by_symbol.find(p_event.symbol_);
        if(itr != s_order_book_by_symbol.end())
        {
          return itr->second->AddOrder(std::move(order));
        }
        OrderBook* new_book = new OrderBook();
        s_order_book_by_symbol.insert_or_assign(p_event.symbol_, new_book);
        return new_book->AddOrder(std::move(order)); 
      }
      break;

     case OrderEvent::Type::Cxl:
      {
        auto itr = s_order_book_by_symbol.find(p_event.symbol_);
        if(itr != s_order_book_by_symbol.end())
        {
          return itr->second->CancelOrder(order);
        }

        //Orig order book not found
        return false;
      }
      break;

      
     case OrderEvent::Type::Rpl:
      {
        auto itr = s_order_book_by_symbol.find(p_event.symbol_);
        if(itr != s_order_book_by_symbol.end())
        {
          return itr->second->ReplaceOrder(order);
        }

        //Orig order book not found
        return false;
      }
      break;
    
    default:
      break;
    }
  }

  private:
    std::map<double, BuyLevel, std::greater<double>> BIDS_;
    std::map<double, SellLevel, std::less<double>> ASKS_;
    static std::unordered_map<std::string, OrderBook*> s_order_book_by_symbol;

    bool Match(const Order& p_order)
    {
      switch (p_order.side_)
      {
      
      case Side::Buy:
      {
        auto itr = ASKS_.find(p_order.price_);
        if(itr != ASKS_.end())
        {
          itr->second.Match(p_order);
        }
        break;
      }

      case Side::Sell:
      {
        auto itr = BIDS_.find(p_order.price_);
        if(itr != BIDS_.end())
        {
          itr->second.Match(p_order);
        }
        break;
      }

      default:
        break;
      }
      return false;
    }

    bool AddOrder(Order&& p_order)
    {
      if(Match(p_order))
        return true;
      
      switch (p_order.side_)
      {
      
      case Side::Buy:
      {
        auto itr = BIDS_.find(p_order.price_);
        if(itr != BIDS_.end())
        {
          return itr->second.AddOrder(std::move(p_order));
        }
        auto price = p_order.price_;
        BuyLevel new_buy_level;
        new_buy_level.price_ = p_order.price_;
        new_buy_level.AddOrder(std::move(p_order));
        BIDS_.insert_or_assign(price, std::move(new_buy_level));
        return true;
      }

      case Side::Sell:
      {
        auto itr = ASKS_.find(p_order.price_);
        if(itr != ASKS_.end())
        {
          return itr->second.AddOrder(std::move(p_order));
        }
        SellLevel new_level;
        new_level.price_ = p_order.price_;
        new_level.AddOrder(std::move(p_order));
        ASKS_.insert_or_assign(p_order.price_, std::move(new_level));
        return true;
      }

      default:
        return false;
      }
    }

  
    bool CancelOrder(const Order& p_order)
    { 
      switch (p_order.side_)
      {
      
      case Side::Buy:
      {
        auto itr = BIDS_.find(p_order.price_);
        if(itr != BIDS_.end())
        {
          return itr->second.CancelOrder(p_order);
        }
        return false;
      }

      case Side::Sell:
      {
        auto itr = ASKS_.find(p_order.price_);
        if(itr != ASKS_.end())
        {
          return itr->second.CancelOrder(p_order);
        }
        return false;
      }

      default:
        return false;
      }
    }

    bool ReplaceOrder(const Order& p_order)
    {
      switch (p_order.side_)
      {
      
      case Side::Buy:
      {
        //FIXME::
        //1. Find old order and cancel that order, this can be done by maintaining another map with order id  and order
        //2. Add New Order
        auto itr = BIDS_.find(p_order.price_); 
        if(itr != BIDS_.end())
        {
          return (itr->second.CancelOrder(p_order) && itr->second.AddOrder(std::move(p_order)));
        }
        return false;
      }

      case Side::Sell:
      {
        auto itr = ASKS_.find(p_order.price_);
        if(itr != ASKS_.end())
        {
          return (itr->second.CancelOrder(p_order) && itr->second.AddOrder(p_order));
        }
        return false;
      }

      default:
        return false;
      }
    }
};
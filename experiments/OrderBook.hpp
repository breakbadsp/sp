#pragma once

#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

#include <sys/time.h>

#include "cmn.hpp"

constexpr size_t TICK_SIZE = 2; //two decimal places, 1 paisa tick size

  enum class Side
  {
    Buy,
    Sell
  };

struct OrderEvent
{
public:
  enum class Type
  {
    New,
    Rpl,
    Cxl
  };

  OrderEvent( int p_id, 
              const std::string& p_symbol,
              double p_price,
              int p_size,
              Type p_type,
              Side p_side)
  : id_(p_id)
  , symbol_(p_symbol)
  , price_(p_price)
  , size_(p_size)
  , type_(p_type)
  , side_(p_side)
  {}

  void Clear()
  {
    id_ = 0;
    symbol_.clear();
    price_ = 0;
    size_ = {0};
    type_ = {Type::New};
    side_ = {Side::Buy};
  }

public:
  int id_ {0};
  std::string symbol_;
  double price_ {0};
  int size_ {0};
  Type type_ {Type::New};
  Side side_ {Side::Buy};
};

struct Order
{
public:
  Order(const OrderEvent& p_event)
    : id_(p_event.id_)
    , price_(p_event.price_)
    , size_(p_event.size_)
    , side_(p_event.side_)
  { 
    gettimeofday(&time_, nullptr);
  }

  //void FromEvent(const OrderEvent& p_event)

  const int id_;
  const double price_{0};
  mutable int size_{0};
  struct timeval time_;
  const Side side_ {Side::Buy};

  bool operator<(const Order &p_rhs) const
  {
    if(time_.tv_sec < p_rhs.time_.tv_sec)
      return true;
    else if(time_.tv_sec > p_rhs.time_.tv_sec)
      return false;

    return time_.tv_usec < p_rhs.time_.tv_usec;
  }

  void Print() const
  {
    std::cout 
      << id_  << ", "
      << price_  << ", "
      << size_  << ", "
      << time_.tv_sec << ":"
      << time_.tv_usec << ", "
      << (int)side_  << std::endl;
  }
};

class Level
{
public:
  Level(Order p_order)
  {
    std::cout << "Order " << p_order.id_ 
      << " Added to new level, price:" << p_order.price_
      << '\n' ;

    size_ += p_order.size_;
    price_ = p_order.price_;
    orders_.insert(p_order);
  }

  bool AddOrder(Order p_order)
  {
    std::cout << "Order id " << p_order.id_ << " Added to price level " << price_ << "\n";
    size_ += p_order.size_;
    orders_.insert(p_order);
    return true;
  }

  bool CancelOrder(const Order& p_order) //FIXME:: optimize
  {
    //TODO:: Handle Level deletion
    //If level removed is TOB, try matching
    for(auto itr = orders_.begin(); itr != orders_.end(); ++itr)
    {
      if(itr->id_ == p_order.id_)
      {
        std::cout << "Order id " << p_order.id_ << " removed from price level " << price_ << "\n";
        size_ -= itr->size_;
        itr = orders_.erase(itr);
        break;
      }
    }
    return true;
  }

  bool Match(Order p_order)
  {
    int remaining_qty = p_order.size_;
    for(auto itr = orders_.begin(); itr != orders_.end();)
    {
      std::cout << "Order " << p_order.id_ << " matched with " << itr->id_ << "\n";
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

  int get_size() const { return size_; }
  double get_price() const { return price_; }

protected:
  int size_{0};
  double price_{0};
  std::set<Order> orders_; //it is sorted by time
};

class BuyLevel : public Level
{
public:
  Side side_{Side::Buy};

  bool operator<(const BuyLevel& p_rhs)
  {
    return get_price() > p_rhs.get_price();
  }
};

class SellLevel : public Level
{
public:
  Side side_{Side::Sell};

  bool operator<(const BuyLevel& p_rhs)
  {
    return get_price() < p_rhs.get_price();
  }
};

class OrderBook
{
public:

  OrderBook(const std::string& p_symbol)
  : symbol_(p_symbol)
  {}

  ~OrderBook()
  {
    std::cout << "Deleting order book \n";
  }

  void Print() const
  {
    std::cout << "==========\nOrder book " 
      << " Symbol:" << symbol_
      << " LTP:" << get_last_traded_price()
      << " BIDprice:" << get_bid_price()
      << " ASKprice:" << get_ask_price()
      << " BIDSize:" << get_bid_size()
      << " ASKSize:" << get_ask_size() << "\n============\n";
  }

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
          itr->second->Print();
          return itr->second->AddOrder((order));
        }
        auto new_book = std::make_unique<OrderBook>(p_event.symbol_);
        auto* ptr = new_book.get();
        s_order_book_by_symbol.insert_or_assign(p_event.symbol_, std::move(new_book));
        return ptr->AddOrder((order));   
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
    return false;
  }

  double get_bid_price() const
  {
    if(!BIDS_.empty())
      return ( BIDS_.cbegin()->first);
    return -1;
  }

  double get_ask_price() const
  {
    if(!ASKS_.empty())
      return ( ASKS_.cbegin()->first);
    return -1;
  }

  double get_bid_size() const
  {
    if(!BIDS_.empty())
      return ( BIDS_.cbegin()->second.get_size());
    return -1;
  }

  double get_ask_size() const
  {
    if(!ASKS_.empty())
      return ( ASKS_.cbegin()->second.get_size());
    return -1;
  }

  double get_last_traded_price() const
  {
    return last_traded_price_;
  }

  private:
    //TODO:: Convert BIDS and ASKS to heap
    std::map<double, BuyLevel, std::greater<double>> BIDS_;
    std::map<double, SellLevel, std::less<double>> ASKS_;
    double last_traded_price_ {0};
    std::string symbol_;
    inline static std::unordered_map<std::string, std::unique_ptr<OrderBook>> s_order_book_by_symbol {};

    bool Match(const Order& p_order)
    {
      switch (p_order.side_)
      {
      case Side::Buy:
        {
          if(sp::almost_equal(get_ask_price(), p_order.price_, TICK_SIZE))
          {
            last_traded_price_ = p_order.price_;
            ASKS_.begin()->second.Match(p_order);
            return true;
          }
          return false;
        }
        break;
      
      case Side::Sell:
        {
          if(sp::almost_equal(get_bid_price(), p_order.price_, TICK_SIZE))
          {
            last_traded_price_ = p_order.price_;
            BIDS_.begin()->second.Match(p_order);
            return true;
          }
          return false;
        }
        break;

      default:
        break;
      }

      return true;
    }

    bool AddOrder(Order p_order)
    {
      std::cout << "Adding below order to order book :\n";
      p_order.Print();

      if(Match(p_order))
        return true;
      
      switch (p_order.side_)
      {
        
      case Side::Buy:
      {
        auto itr = BIDS_.find(p_order.price_);
        if(itr == BIDS_.end())
        {
          //Create Level
          if( !(BIDS_.emplace(p_order.price_, p_order)).second )
          {
            std::cerr << "Fatal order insertion in order book failed\n";
            return false;
          }
          return true;
        }
        if(!itr->second.AddOrder(p_order))
        {
          return false;
        }
        break;
      }

      case Side::Sell:
      {
        auto itr = ASKS_.find(p_order.price_);
        if(itr == ASKS_.end())
        {
          //Create Level
          if( !(ASKS_.emplace(p_order.price_, p_order).second ) )
          {
            std::cerr << "Fatal order insertion in order book failed\n";
            return false;
          }
          return true;
        }
        if(!itr->second.AddOrder(p_order))
        {
          return false;
        }
        break;
      }

      default:
        break;
      }
      return false;
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
          return (itr->second.CancelOrder(p_order) && itr->second.AddOrder((p_order)));
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
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using Qty = int32_t;
using Price = double;
using Symbol = std::string;
using OrderId = int32_t;

constexpr bool debug = true;

struct Order {
  enum class Side { Buy = 1, Sell };
  OrderId id_;
  Qty qty_;
  Price price_;
  Symbol sym_;
  Side side_;
};

class OrderBook {
 public:

  struct Exec {
    enum OrderState {
      Failed = 0,
      Ack,
      Cxld,
      Filled,
      Part
    };

    std::vector<Order> matched_ords_ {}; // for testing
    Qty exec_qty_ {0};
    OrderState ostat_{OrderState::Failed};
  };

  struct Level {
    Level(const Order& p_order)
        : price_(p_order.price_), volume_(p_order.qty_) {

      std::cout << "Added order " << p_order.id_ << "@" << price_ <<  ", volume: " 
        << volume_ << '\n';
      orders_.push_back(p_order);
    }

    void AddOrder(const Order& p_order) {
      assert(p_order.price_ == price_);
      orders_.push_back(p_order);
      volume_ += p_order.qty_;
      std::cout << "Added order " << p_order.id_ << "@" << price_ <<  ", volume: " 
        << volume_ << '\n';
    }

    void FillQty(Qty& rem_qty, std::vector<Order>& p_matched) {
      for(auto i = orders_.begin(); rem_qty > 0 && i != orders_.end();) {  
        p_matched.push_back(*i);
        /*if(debug) std::cout << "matching order found: " << i->id_ << " qty: " << i->qty_ 
          << ", total orders matched:" 
          << p_matched.size() << ", rem_qty: " << rem_qty <<  ", volume left:" << volume_ << '\n';*/
        if (i->qty_ < rem_qty) {
          volume_ -= i->qty_;  
          rem_qty -= i->qty_;
          i = orders_.erase(i);
          continue;
        }

        volume_ -= i->qty_;
        i->qty_ -= rem_qty;
        if (i->qty_ == 0) 
          i = orders_.erase(i);
        else
          ++i;
        rem_qty = 0;
        break;
      }
    }

    Qty get_volume() const { return volume_; }

    Price price_;
    Qty volume_;
    std::vector<Order> orders_;
  };

  Exec AddOrder(const Order& p_order) {
    [[maybe_unused]] Qty unfilled_qty = p_order.qty_;
    
    Exec exec_re;
    if (MatchOrder(p_order, unfilled_qty, exec_re.matched_ords_)) {
      
      exec_re.ostat_ = unfilled_qty == 0 ? Exec::OrderState::Filled 
                                         : Exec::OrderState::Part;
      exec_re.exec_qty_ = p_order.qty_ - unfilled_qty;

      if(debug) std::cout << "New Order " << p_order.id_ << " filled with remaining qty " 
        << unfilled_qty << ", filled: " << exec_re.exec_qty_
        << " bid:" << get_bid() << ", ask:" << get_ask() << '\n';

      if(debug) std::cout << "Matched order ids: ";
      for(auto order: exec_re.matched_ords_)
        if(debug) std::cout << order.id_  << ", price:" << order.price_ << ' ';
      if(debug) std::cout << '\n';
        
      if(exec_re.ostat_  == Exec::OrderState::Filled)
        return exec_re;

      auto updated_order = p_order;
      updated_order.qty_ = unfilled_qty;
      StoreOrder(updated_order);
      return exec_re;
    }

    //if(debug) std::cout << "New Order " << p_order.id_ << " added, bid:" << get_bid() << ", ask:" << get_ask() << '\n';
    StoreOrder(p_order);
    exec_re.ostat_ = Exec::OrderState::Ack;
    return exec_re;
  }

  void StoreOrder(const Order& p_order) {
    switch (p_order.side_) {
      case Order::Side::Buy: {
        auto itr = std::lower_bound(
            bids_.begin(), bids_.end(), p_order.price_,
            [](const auto& lhs, auto rhs) { return lhs.price_ < rhs; });

        if (itr == bids_.end()) {
          bids_.emplace_back(p_order);
        } else if (itr->price_ > p_order.price_) {
          bids_.insert(itr, Level(p_order));
        } else {
          itr->AddOrder(p_order);
        }

        std::sort(bids_.begin(), bids_.end(),
                  [](const auto& lhs, const auto& rhs) {
                    return lhs.price_ < rhs.price_;
                  });
      }
      break;

      case Order::Side::Sell: {
        auto itr = std::lower_bound(
            asks_.begin(), asks_.end(), p_order.price_,
            [](const auto& lhs, auto rhs) { return lhs.price_ > rhs; });

        if (itr == asks_.end()) {
          asks_.emplace_back(p_order);
        } else if (itr->price_ < p_order.price_) {
          asks_.insert(itr, Level(p_order));
        } else {
          itr->AddOrder(p_order);
        }

        std::sort(asks_.begin(), asks_.end(),
                  [](const auto& lhs, const auto& rhs) {
                    return lhs.price_ > rhs.price_;
                  });
      }
      break;
    }
  }

  bool MatchOrder(const Order& p_order, Qty& rem_qty,
                  std::vector<Order>& p_matched) 
  {
    //if(debug) std::cout << "Matching order: " << p_order.id_ << ", Price " << p_order.price_ << '\n';
    std::vector<Level>& opp_levels = p_order.side_ == Order::Side::Sell ? bids_ : asks_;

    if (opp_levels.empty()) {
      //if(debug) std::cout << "Empty opp level \n";
      return false;
    }

    switch (p_order.side_) {
      case Order::Side::Sell: {
        if (get_bid() < p_order.price_) {
          //if(debug) std::cout << "bid:" << get_bid() << " is less than sell price:" << p_order.price_  <<  "\n";
          return false;
        }
      }
      break;

      case Order::Side::Buy: {
        if (get_ask() > p_order.price_) {
          //if(debug) std::cout << "ask:" << get_ask() << " is greater than buy price:" << p_order.price_  <<  "\n";
          return false;
        }
      }
      break;
    }

    auto itr = opp_levels.rbegin();
    while (rem_qty > 0 && 
           itr != opp_levels.rend() ) 
    {
      bool finished = false;
      switch (p_order.side_) {
        case Order::Side::Sell:
          if(itr->price_ < p_order.price_)
            finished = true;
        break;
        
        case Order::Side::Buy:
          if(itr->price_ > p_order.price_)
            finished = true;
        break;
      }

      if(finished)
        break;

      itr->FillQty(rem_qty, p_matched);
      ltp_ = itr->price_;
      std::cout << "Search in level " << itr->price_ << "---@addr:" << &(itr->orders_)
        << "---for order:" << p_order.id_ << " is done, left to match:" << rem_qty
        << " left in level " << itr->volume_ <<  '\n';
      if(itr->volume_ == 0 || itr->orders_.empty()) {
          opp_levels.pop_back();
          itr = opp_levels.rbegin();
          continue;
      }
      ++itr;
    }
    if(debug) std::cout << "total orders matched: " << p_matched.size() <<'\n';
    return true;
  }

  Price get_ask() const { return asks_.empty() ? 0.0 : asks_.back().price_; }
  Price get_bid() const { return bids_.empty() ? 0.0 : bids_.back().price_; }
  Price get_ltp() const {return ltp_; }

 private:
  std::vector<Level> bids_;
  std::vector<Level> asks_;
  Price ltp_;
};

int main() {
  OrderBook book;

  book.AddOrder(
      Order{OrderId{12}, Qty{12}, Price{98}, Symbol{"GOOG"}, Order::Side::Buy});
  assert(book.get_bid() == 98);
  book.AddOrder(
      Order{OrderId{11}, Qty{11}, Price{99}, Symbol{"GOOG"}, Order::Side::Buy});
  assert(book.get_bid() == 99);
  book.AddOrder(
      Order{OrderId{13}, Qty{13}, Price{97}, Symbol{"GOOG"}, Order::Side::Buy});
  assert(book.get_bid() == 99);
  book.AddOrder(
      Order{OrderId{14}, Qty{14}, Price{96}, Symbol{"GOOG"}, Order::Side::Buy});
  assert(book.get_bid() == 99);
  book.AddOrder(
      Order{OrderId{15}, Qty{15}, Price{95}, Symbol{"GOOG"}, Order::Side::Buy});
  assert(book.get_bid() == 99);
  book.AddOrder(Order{OrderId{15}, Qty{15}, Price{99.5}, Symbol{"GOOG"},
                      Order::Side::Buy});
  book.AddOrder(Order{OrderId{151}, Qty{15}, Price{99.5}, Symbol{"GOOG"},
                      Order::Side::Buy});
  book.AddOrder(Order{OrderId{152}, Qty{15}, Price{99.5}, Symbol{"GOOG"},
                      Order::Side::Buy});
  book.AddOrder(Order{OrderId{153}, Qty{15}, Price{99.5}, Symbol{"GOOG"},
                      Order::Side::Buy});  
  assert(book.get_bid() == 99.5);

  book.AddOrder(Order{OrderId{21}, Qty{21}, Price{101}, Symbol{"GOOG"},
                      Order::Side::Sell});
  assert(book.get_ask() == 101);
  book.AddOrder(Order{OrderId{22}, Qty{22}, Price{102}, Symbol{"GOOG"},
                      Order::Side::Sell});
  assert(book.get_ask() == 101);
  book.AddOrder(Order{OrderId{23}, Qty{23}, Price{103}, Symbol{"GOOG"},
                      Order::Side::Sell});
  assert(book.get_ask() == 101);
  book.AddOrder(Order{OrderId{24}, Qty{24}, Price{104}, Symbol{"GOOG"},
                      Order::Side::Sell});
  assert(book.get_ask() == 101);
  book.AddOrder(Order{OrderId{25}, Qty{25}, Price{105}, Symbol{"GOOG"},
                      Order::Side::Sell});
  assert(book.get_ask() == 101);
  book.AddOrder(Order{OrderId{25}, Qty{25}, Price{100.5}, Symbol{"GOOG"},
                      Order::Side::Sell});
  assert(book.get_ask() == 100.5);

  assert(book.get_ask() == 100.5);
  assert(book.get_bid() == 99.5);

  auto exec = book.AddOrder(Order{OrderId{26}, Qty{100}, Price{98},
                                    Symbol{"GOOG"}, Order::Side::Sell});
  assert(exec.ostat_ == OrderBook::Exec::OrderState::Filled);
  assert(exec.exec_qty_ == 70);
  assert(book.get_bid() == 99);
  assert(book.get_ask() == 100.5);
  assert(book.get_ltp() == 99);

  auto exec1 = book.AddOrder(Order{OrderId{27}, Qty{25}, Price{100.5},
                                    Symbol{"GOOG"}, Order::Side::Buy});
  assert(exec1.ostat_ == OrderBook::Exec::OrderState::Filled);
  assert(exec1.exec_qty_ == 25);
  assert(book.get_ask() == 101);
  assert(book.get_ltp() == 100.5);
  assert(book.get_bid() == 99);
  if(debug) std::cout << "Testing Order book finished\n";
}
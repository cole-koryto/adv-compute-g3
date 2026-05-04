#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP
#include <algorithm>
#include <limits>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

struct Order {
  int id;
  double price;
  int quantity;
  char side; // 'B' for bid, 'S' for ask
};

class OrderBook {

  std::unordered_map<int, Order> orders_by_id;                   // id -> order
  std::map<double, std::vector<int>, std::greater<double>> bids; // price -> id
  std::map<double, std::vector<int>> asks;

public:
  void add_order(int id, double price, int quantity, char side) {

    if (side == 'B')
      bids[price].push_back(id);
    else if (side == 'S')
      asks[price].push_back(id);
    else
      return;

    orders_by_id[id] = {id, price, quantity, side};
  }

  void cancel_order(int id) {
    auto it = orders_by_id.find(id);
    if (it == orders_by_id.end())
      return;

    const Order &order = it->second;

    auto &level = (order.side == 'B') ? bids[order.price] : asks[order.price];
    level.erase(std::find(level.begin(), level.end(), order.id));

    if (level.empty()) {
      if (order.side == 'B') {
        bids.erase(order.price);
      } else
        asks.erase(order.price);
    }

    orders_by_id.erase(it);
  }

  void modify_quantity(int id, int quantity) {
    auto it = orders_by_id.find(id);
    if (it == orders_by_id.end())
      return;

    Order &order = it->second;
    order.quantity = quantity;
  }

  std::optional<double> best_bid() const {
    return bids.empty() ? std::nullopt
                        : std::optional<double>(bids.begin()->first);
  }

  std::optional<double> best_ask() const {
    return asks.empty() ? std::nullopt
                        : std::optional<double>(asks.begin()->first);
  }
};

class OrderBookNaive {
  std::unordered_map<int, Order> orders_by_id;

public:
  void add_order(int id, double price, int quantity, char side) {

    orders_by_id[id] = {id, price, quantity, side};
  }

  void cancel_order(int id) {
    auto it = orders_by_id.find(id);
    if (it == orders_by_id.end())
      return;

    const Order &order = it->second;

    orders_by_id.erase(it);
  }

  void modify_quantity(int id, int quantity) {
    auto it = orders_by_id.find(id);
    if (it == orders_by_id.end())
      return;

    Order &order = it->second;
    order.quantity = quantity;
  }

  std::optional<double> best_bid() const {
    double best = 0.0;
    bool found = false;
    for (const auto &[id, order] : orders_by_id) {
      if (order.side == 'B' && (!found || order.price > best)) {
        best = order.price;
        found = true;
      }
    }
    return found ? std::optional<double>(best) : std::nullopt;
  }

  std::optional<double> best_ask() const {
    double best = std::numeric_limits<double>::max();
    bool found = false;
    for (const auto &[id, order] : orders_by_id) {
      if (order.side == 'S' && (!found || order.price < best)) {
        best = order.price;
        found = true;
      }
    }
    return found ? std::optional<double>(best) : std::nullopt;
  }
};
#endif // ORDER_BOOK_HPP

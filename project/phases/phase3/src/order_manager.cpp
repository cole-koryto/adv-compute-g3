#include "order_manager.h"
#include <memory>
#include <iostream>

int OrderManager::place_order(Side side, double price, int qty)
{

    std::unique_ptr<MyOrder> order = std::make_unique<MyOrder>();

    // set order vals
    order->id = next_order_id++;
    order->side = side;
    order->quantity = qty;
    order->price = price;

    // rest are default

    // store order
    int id = order->id;

    // move order into map to avoid copying
    orders[id] = std::move(order);

    return id;
}

void OrderManager::cancel(int id)
{
    orders.erase(id);
}

void OrderManager::handle_fill(int id, int filled_qty)
{

    if (orders.find(id) == orders.end())
        return;

    orders[id]->filled += filled_qty;

    if (orders[id]->filled >= orders[id]->quantity)
        orders.erase(id);
    else
        orders[id]->status = OrderStatus::PartiallyFilled;
}

void OrderManager::print_active_orders() const
{
    for (const auto &[id, order] : orders)
    {
        std::cout << "order " << id << ": "
                  << "qty: " << order->quantity
                  << ", price: " << order->price
                  << ", filled: " << order->filled << std::endl;
    }
}
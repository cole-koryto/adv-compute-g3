#include "../include/order_book.hpp"

#include <cassert>
#include <iostream>

int main()
{
    hft::phase5::OrderBook book;

    book.add_order("ORD001", 50.10, 100, true);
    book.add_order("ORD002", 50.20, 200, true);
    book.add_order("ORD003", 50.15, 150, false);

    assert(book.size() == 3);
    assert(book.find("ORD001") != nullptr);
    assert(book.find("ORD999") == nullptr);

    // Modify
    assert(book.modify_order("ORD001", 51.00, 75));
    const auto *o = book.find("ORD001");
    assert(o != nullptr);
    assert(o->price == 51.00);
    assert(o->quantity == 75);

    // Delete
    assert(book.delete_order("ORD002"));
    assert(book.find("ORD002") == nullptr);
    assert(book.size() == 2);

    // Idempotent-ish behavior
    assert(!book.delete_order("ORD002"));
    assert(!book.modify_order("ORD404", 1.0, 1));

    std::cout << "phase5_test OK\n";
    return 0;
}


#ifndef TRADE_H
#define TRADE_H

#include <string>
#include <iostream>

struct Trade
{
    std::string symbol;
    double price;

    Trade(const std::string &s, double p) : symbol(s), price(p)
    {
        std::cout << "Trade created: " << symbol << "\n";
    }

    ~Trade()
    {
        std::cout << "Trade destroyed: " << symbol << "\n";
    }
};

class TradeHandle
{
    Trade *ptr;

public:
    TradeHandle(Trade *p);
    ~TradeHandle();

    // delete default copy constructor and copy assignment operator to prevent copying of TradeHandle, which would lead to double deletion issues.
    TradeHandle(const TradeHandle &) = delete;
    TradeHandle &operator=(const TradeHandle &) = delete;

    // move constructor
    TradeHandle(TradeHandle &&other);

    // move assignment operator
    TradeHandle &operator=(TradeHandle &&other);

    // arrow operator to access members of Trade through TradeHandle
    Trade *operator->();

    // dereference operator to access the Trade object itself
    Trade &operator*();
};

#endif // TRADE_H
#include <iostream>
#include <string>
#include <vector>
#include "trade.h"

int main()
{

    TradeHandle t1(new Trade("AAPL", 150.0));
    std::cout << "t1 symbol: " << t1->symbol << ", price"
              << t1->price << "\n";

    return 0;
}
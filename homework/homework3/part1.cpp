#include <iostream>
#include <string>
#include <vector>
#include "trade.h"

int main()
{

    Trade *t1 = new Trade("AAPL", 150.0);

    std::cout << "t1 symbol: " << t1->symbol << ", price: " << t1->price << "\n";

    Trade *trades = new Trade[5]{
        {"NVDA", 900.0},
        {"AMZN", 3200.0},
        {"META", 250.0},
        {"TSLA", 750.0},
        {"MSFT", 300.0}};

    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Trade " << i + 1 << ": " << trades[i].symbol << ", price: " << trades[i].price << "\n";
    }

    delete t1;
    delete[] trades;
}

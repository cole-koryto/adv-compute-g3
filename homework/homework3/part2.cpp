#include <iostream>
#include <string>
#include <vector>
#include "trade.h"

int main()
{
    Trade *t1 = new Trade("AAPL", 150.0);
    Trade *t2 = new Trade("GOOG", 2800.0);
    delete t1;
    // delete t1; // ❌ this is wrong.
    delete t2; // also need to delete t2.

    Trade *t3 = new Trade("MSFT", 300.0);
    // t3 = new Trade("TSLA", 750.0); // ❌ memory leak. new returns a pointer to newly allocated memory, so we lose the reference to the original memory allocated for "MSFT", and we can't delete it anymore.
    delete t3; // also need to delte.

    Trade *trades = new Trade[3]{
        {"NVDA", 900.0},
        {"AMZN", 3200.0},
        {"META", 250.0}};

    // delete trades; // ❌ error: deleting array of objects, should use delete[].
    delete[] trades;

    return 0;
}

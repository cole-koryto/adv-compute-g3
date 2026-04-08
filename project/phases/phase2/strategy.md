# Phase 2 - Momentum-Based Smart Order Client
## Strategy Overview

For this phase, we modified the `hft_client.cpp` code so that it would execute trades based on
a momentum strategy. The client just constructs a simple order only based on the price. Currently
the server does not support buy or sell orders. The server only allows one client to execute a trade
on the given price at a time, but this does not affect the logic flow for the client code itself.

We implemented this momentum trading strategy by saving a deque of the last three market prices.
The client will not try to detect momentum or make trades before three prices are saved. Momentum is
detected in either the up or down state. The up state is defined as $price_{t-2} < price_{t-1} \land price_{t-1} < price_{t}$.
The down state is defined as $price_{t-2} > price_{t-1} \land price_{t-1} > price_{t}$. Once up or down momentum is detected,
a trade order is sent to the server to be logged. 
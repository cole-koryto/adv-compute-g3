#include "feed_parser.h"
#include "market_snapshot.h"
#include "order_manager.h"

// example loop
/*
for (auto& update : load_feed("sample_feed.txt")) {
    snapshot.update_bid(update.bid_price, update.bid_qty);
    snapshot.update_ask(update.ask_price, update.ask_qty);

    if (should_trade(snapshot)) {
        int id = om.place_order(Side::Buy, snapshot.get_best_bid()->price, 10);
        std::cout << "Placed BUY order at " << snapshot.get_best_bid()->price << "\n";
    }

    if (update.filled_order_id != -1) {
        om.handle_fill(update.filled_order_id, update.qty);
    }
}
*/

int main()
{
    auto feed = load_feed("sample_feed.txt");
    MarketSnapshot snapshot = MarketSnapshot();
    OrderManager om = OrderManager();

    int t = 0;

    for (const auto &event : feed)
    {
        std::cout << "t = " << t << " \n";

        event.print();

        // Integrate with your components:
        if (event.type == FeedType::BID)
        {

            snapshot.update_bid(event.price, event.quantity);
        }
        else if (event.type == FeedType::ASK)
        {
            snapshot.update_ask(event.price, event.quantity);
            const PriceLevel *ask = snapshot.get_best_ask();
            if (ask && ask->price == 100.2)
            {
                int id = om.place_order(Side::Buy, ask->price, 50);
                std::cout << "Placed BUY order at " << ask->price << "\n";
            }
        }
        else if (event.type == FeedType::EXECUTION)
        {
            om.handle_fill(event.order_id, event.quantity);
        }

        std::cout << "Active orders:\n";
        om.print_active_orders();

        t++;
    }

    // int *x = new int(5);
    return 0;
}
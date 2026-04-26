#include "../include/OrderManager.hpp"

namespace hft::phase4
{
template <typename OrderIdType>
void OrderManager<OrderIdType>::on_new(OrderIdType id, int quantity)
{
    states_[id] = State{OrderStatus::New, quantity, 0};
}

template <typename OrderIdType>
void OrderManager<OrderIdType>::on_cancel(OrderIdType id)
{
    const auto it = states_.find(id);
    if (it == states_.end())
        return;
    it->second.status = OrderStatus::Cancelled;
}

template <typename OrderIdType>
void OrderManager<OrderIdType>::on_fill(OrderIdType id, int fill_qty)
{
    const auto it = states_.find(id);
    if (it == states_.end())
        return;
    it->second.filled_qty += fill_qty;
    if (it->second.filled_qty >= it->second.original_qty)
        it->second.status = OrderStatus::Filled;
    else
        it->second.status = OrderStatus::PartiallyFilled;
}

template <typename OrderIdType>
const typename OrderManager<OrderIdType>::State *OrderManager<OrderIdType>::get(OrderIdType id) const
{
    const auto it = states_.find(id);
    if (it == states_.end())
        return nullptr;
    return &it->second;
}

template class OrderManager<int>;
template class OrderManager<std::int64_t>;
} // namespace hft::phase4


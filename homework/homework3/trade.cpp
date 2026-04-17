#include "trade.h"

TradeHandle::TradeHandle(Trade *p) : ptr(p) {}

TradeHandle::~TradeHandle()
{
    delete ptr;
}

TradeHandle::TradeHandle(TradeHandle &&other) : ptr(other.ptr)
{
    other.ptr = nullptr;
}

TradeHandle &TradeHandle::operator=(TradeHandle &&other)
{
    if (this != &other)
    {
        delete ptr;          // free existing resource
        ptr = other.ptr;     // transfer ownership
        other.ptr = nullptr; // prevent double deletion
    }
    return *this;
}

// arrow operator to access members of Trade through TradeHandle
Trade *TradeHandle::operator->()
{
    return ptr;
}

// dereference operator to access the Trade object itself
Trade &TradeHandle::operator*()
{
    return *ptr;
}

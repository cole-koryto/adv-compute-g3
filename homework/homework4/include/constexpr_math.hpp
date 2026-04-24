#pragma once

constexpr double price_bucket(double price)
{
    return static_cast<int>(price / 0.05) * 0.05;
}

constexpr int square(int x)
{
    return x * x;
}

constexpr int Size = square(5); int arr[Size];


constexpr int factorial(int x)
{
    if (x == 0) return 1;
    return x * factorial(x - 1);
}

constexpr int fibonacci(int x)
{
    if (x == 0) return 0;
    if (x == 1) return 1;
    return fibonacci(x - 1) + fibonacci(x - 2);
}
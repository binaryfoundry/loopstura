#pragma once

#include <cstdint>

constexpr double power(double base, int exp)
{
    if (exp < 0)
    {
        if (base == 0)
            return -0; // Error!!
        return 1 / (base * power(base, (-exp) - 1));
    }
    if (exp == 0)
        return 1;
    if (exp == 1)
        return base;
    return base * power(base, exp - 1);
}

constexpr uint64_t fact(int n)
{
    return n <= 0 ? 1 : n * fact(n - 1);
}

constexpr double cosine(double rad)
{
    double cos = 0;

    for (int i = 0; i < 8; i++)
    {
        cos += power(-1, i) * power(rad, 2 * i) / fact(2 * i);
    }
    return cos;
}

template<size_t S>
struct HannWindow
{
    double arr[S];
    constexpr HannWindow() : arr()
    {
        for (size_t i = 0; i < S; i++)
        {
            arr[i] = 0.5 * (1.0 - cosine(2.0 * 3.14159265358979323846 * i / (double)(S - 1)));
        }
    }
};

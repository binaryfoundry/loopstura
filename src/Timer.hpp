#pragma once

#include <chrono>

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point timer_start_point;

public:

    Timer()
    {
        Start();
    }

    void Start()
    {
        timer_start_point = std::chrono::high_resolution_clock::now();
    }

    float End()
    {
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(end - timer_start_point);
        return time_span.count();
    }
};

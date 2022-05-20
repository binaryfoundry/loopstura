#include "Waveform.hpp"

#include <cmath>
#include <iostream>

Waveform::Waveform(
    const size_t size) :
    size(size)
{
    Resize(size);
}

void Waveform::Resize(const size_t new_size)
{
    size = new_size;
    x_data.resize(size);
    min_data.resize(size);
    max_data.resize(size);
}

void Waveform::Slice(double position, std::vector<float>& data, std::vector<float>& window)
{
    uint32_t offset = std::floor(position * data.size());

    for (size_t i = 0; i < window.size(); i++)
    {
        window[i] = data[(i + offset) % size];
    }
}

void Waveform::FillX(double position, std::vector<float>& window_x)
{
    Slice(position, x_data, window_x);
}

void Waveform::FillYMax(double position, std::vector<float>& window_y_max)
{
    Slice(position, min_data, window_y_max);
}

void Waveform::FillYMin(double position, std::vector<float>& window_y_min)
{
    Slice(position, max_data, window_y_min);
}

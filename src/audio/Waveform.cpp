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

void Waveform::Fill(double position, std::vector<float>& window)
{
    uint32_t offset = std::floor(position * min_data.size());

    for (size_t i = 0; i < window.size() / 4; i++)
    {
        window[(i * 4) + 0] = max_data[(i + offset) % size];
        window[(i * 4) + 1] = -min_data[(i + offset) % size];
    }
}

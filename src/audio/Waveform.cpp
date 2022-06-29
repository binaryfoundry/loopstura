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

void Waveform::Fill(double position, const float speed, std::vector<float>& window)
{
    const double start = position * min_data.size();

    for (size_t i = 0; i < window.size() / 4; i++)
    {
        const uint32_t offset = static_cast<uint32_t>(
            std::floor(start + (speed * i)));

        window[(i * 4) + 0] = max_data[(offset) % size];
        window[(i * 4) + 1] = -min_data[(offset) % size];
    }
}

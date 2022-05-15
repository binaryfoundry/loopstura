#include "Waveform.hpp"

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

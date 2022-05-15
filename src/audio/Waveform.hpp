#pragma once

#include <vector>
#include <memory>

class Waveform
{
private:
    size_t size;


public:
    Waveform(
        const size_t size);

    std::vector<float> x_data;
    std::vector<float> min_data;
    std::vector<float> max_data;
    size_t const Size() const
    {
        return size;
    }

    void Resize(const size_t size);
};
